#include "fft/battle_ai.h"
#include "fft/memory.h"

/* Evaluate the acting unit's Math Skill, used from its own tile before moving. For each learned
 * Calculator-eligible ability of the four source skillsets (White, Black, Time and Yin-Yang Magic)
 * that is useful on some unit (battle_ai_keep_useful_targets), every learned type (CT,
 * Level, Exp, Height) is scored with every learned multiplier (Prime Number, 5, 4, 3), and the best is
 * ranked (battle_ai_insert_ranked_action). Returns -1 to suspend, else 0.
 *
 * The extra known-ability call for the listed special ids discards its result (QUIRKS.md). */
s32 battle_ai_evaluate_math_targets(void) {
    u8 special_abilities[BATTLE_AI_MATH_EXTRA_KNOWN_ABILITY_COUNT];
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 i;
    s32 unit;

    memcpy(special_abilities, g_battle_ai_math_extra_known_ability_ids, sizeof(special_abilities));
    if (g_battle_ai_data_base.resume_pending)
        goto evaluate_priority; /* resume inside the multiplier loop */
    if (!battle_ai_can_use_ability_at_coords(&ai->acting_unit_coords))
        return 0;
    g_battle_ai_data_base.current_action.act_before_move = 1;
    for (g_battle_ai_math_skillset_index = 0; g_battle_ai_math_skillset_index < BATTLE_AI_MATH_SOURCE_SKILLSET_COUNT;
        g_battle_ai_math_skillset_index++) {
        g_battle_ai_math_skillset_id = g_battle_ai_math_source_skillsets[g_battle_ai_math_skillset_index];
        g_battle_ai_math_ability_list
            = main_ability_store_skillset_abilities(g_battle_ai_math_skillset_id, SKILLSET_ABILITY_FILTER_ACTION);
        for (g_battle_ai_math_ability_index = 0; g_battle_ai_math_ability_index < RSM_SKILLSET_SLOT_FIRST;
            g_battle_ai_math_ability_index++) {
            /* Math targeting reads only the low byte of each 16-bit ID. */
            ai->math_ability_id = g_battle_ai_math_ability_list[g_battle_ai_math_ability_index];
            if (ai->math_ability_id == 0)
                continue;
            if (!(g_main_ability_range_data[ai->math_ability_id].flags_3
                    & ABILITY_SECONDARY_FLAG_3_CALCULATOR_ELIGIBLE))
                continue;
            if (!battle_ai_is_ability_known(
                    ai->acting_unit_id, g_battle_ai_math_skillset_id, g_battle_ai_math_ability_index))
                continue;
            /* The extra known-bit call's result is ignored. */
            for (i = 0; i < BATTLE_AI_MATH_EXTRA_KNOWN_ABILITY_COUNT; i++) {
                if (ai->math_ability_id == special_abilities[i])
                    battle_ai_is_ability_known(
                        ai->acting_unit_id, g_battle_ai_math_skillset_id, g_battle_ai_math_ability_index + 1);
            }
            battle_mem_copy_u16_values(
                &ai->considered_ability.ai_flags, &g_main_ability_data[ai->math_ability_id].ai_flags, 4);
            battle_mem_copy_bytes(ai->considered_ability.parameters.fields.status_infliction,
                g_main_status_infliction_data[g_main_ability_range_data[ai->math_ability_id].status_infliction_id]
                    .statuses,
                5);
            ai->considered_ability.ai_flags.fields.flags_4 = BATTLE_AI_ABILITY_FLAG_4_USABLE_BY_AI;
            ai->current_action.command.ability_id = ai->math_ability_id;
            ai->considered_ability.element = g_main_ability_range_data[ai->math_ability_id].element;
            for (unit = 0; unit < BATTLE_UNIT_SLOT_COUNT; unit++)
                ai->targetability.live.unit_targetable[unit] = ai->targetability.live.unit_active[unit];
            if (!battle_ai_keep_useful_targets())
                continue;
            for (ai->math_type_index = 0; ai->math_type_index < CALCULATOR_ATTRIBUTE_ABILITY_COUNT;
                ai->math_type_index++) {
                if (!battle_ai_is_ability_known(
                        ai->acting_unit_id, ai->considered_ability.skillset, ai->math_type_index))
                    continue;
                ai->math_multiplier_index = CALCULATOR_ATTRIBUTE_ABILITY_COUNT;
                ai->current_action.command.calculator_type = ai->math_type_index + ABILITY_ID_MATH_FIRST;
                do {
                    if (battle_ai_is_ability_known(
                            ai->acting_unit_id, ai->considered_ability.skillset, ai->math_multiplier_index)) {
                        ai->current_action.command.calculator_multiplier
                            = ai->math_multiplier_index + ABILITY_ID_MATH_FIRST;
                    evaluate_priority:
                        if (battle_ai_score_current_action() == -1)
                            return -1;
                    }
                    ai->math_multiplier_index++;
                } while (ai->math_multiplier_index < CALCULATOR_ABILITY_COUNT);
            }
            battle_ai_insert_ranked_action();
        }
    }
    battle_ai_move_temp_unit_to_coords(&ai->acting_unit_coords);
    return 0;
}
