#include "fft/battle_ai.h"
#include "fft/memory.h"

/*
 * Evaluate learned Math Skill ability and parameter combinations.
 *
 * Test four source skillsets and learned type/multiplier pairs. Persistent
 * indices resume a suspended priority check; -1 suspends and 0 ends this pass.
 */
s32 ai_evaluate_math_targets(void) {
    u8 special_abilities[AI_MATH_EXTRA_KNOWN_ABILITY_COUNT];
    ai_data_t* ai = &g_ai_data_base;
    s32 i;
    s32 unit;

    memcpy(special_abilities, g_ai_math_extra_known_ability_ids, sizeof(special_abilities));
    if (g_ai_data_base.resume_pending)
        goto evaluate_priority; /* resume inside the multiplier loop */
    if (!ai_is_ability_origin_allowed(&ai->acting_unit_coords))
        return 0;
    g_ai_data_base.current_action.act_before_move = 1;
    for (g_ai_math_skillset_index = 0; g_ai_math_skillset_index < AI_MATH_SOURCE_SKILLSET_COUNT;
        g_ai_math_skillset_index++) {
        g_ai_math_skillset_id = g_ai_math_source_skillsets[g_ai_math_skillset_index];
        g_ai_math_ability_list
            = main_ability_store_skillset_abilities(g_ai_math_skillset_id, SKILLSET_ABILITY_FILTER_ACTION);
        for (g_ai_math_ability_index = 0; g_ai_math_ability_index < HUMAN_SKILLSET_ACTION_SLOT_COUNT;
            g_ai_math_ability_index++) {
            /* Math targeting reads only the low byte of each 16-bit ID. */
            ai->math_ability_id = g_ai_math_ability_list[g_ai_math_ability_index];
            if (ai->math_ability_id == 0)
                continue;
            if (!(g_main_ability_range_data[ai->math_ability_id].flags_3
                    & ABILITY_SECONDARY_FLAG_3_CALCULATOR_ELIGIBLE))
                continue;
            if (!ai_is_ability_known(ai->acting_unit_id, g_ai_math_skillset_id, g_ai_math_ability_index))
                continue;
            /* The extra known-bit call's result is ignored. */
            for (i = 0; i < AI_MATH_EXTRA_KNOWN_ABILITY_COUNT; i++) {
                if (ai->math_ability_id == special_abilities[i])
                    ai_is_ability_known(ai->acting_unit_id, g_ai_math_skillset_id, g_ai_math_ability_index + 1);
            }
            mem_copy_u16_values(
                &ai->considered_ability.ai_flags, &g_main_ability_data[ai->math_ability_id].ai_flags, 4);
            mem_copy_bytes(ai->considered_ability.parameters.fields.status_infliction,
                g_main_status_infliction_data[g_main_ability_range_data[ai->math_ability_id].status_infliction_id]
                    .statuses,
                5);
            ai->considered_ability.ai_flags.fields.flags_4 = AI_ABILITY_FLAG_4_USABLE_BY_AI;
            ai->current_action.command.ability_id = ai->math_ability_id;
            ai->considered_ability.element = g_main_ability_range_data[ai->math_ability_id].element;
            for (unit = 0; unit < BATTLE_UNIT_SLOT_COUNT; unit++)
                ai->targetability.live.unit_targetable[unit] = ai->targetability.live.unit_active[unit];
            if (!ai_filter_useful_targets())
                continue;
            for (ai->math_type_index = 0; ai->math_type_index < CALCULATOR_ATTRIBUTE_ABILITY_COUNT;
                ai->math_type_index++) {
                if (!ai_is_ability_known(ai->acting_unit_id, ai->considered_ability.skillset, ai->math_type_index))
                    continue;
                ai->math_multiplier_index = CALCULATOR_ATTRIBUTE_ABILITY_COUNT;
                ai->current_action.command.calculator_type = ai->math_type_index + ABILITY_ID_MATH_FIRST;
                do {
                    if (ai_is_ability_known(
                            ai->acting_unit_id, ai->considered_ability.skillset, ai->math_multiplier_index)) {
                        ai->current_action.command.calculator_multiplier
                            = ai->math_multiplier_index + ABILITY_ID_MATH_FIRST;
                    evaluate_priority:
                        if (ai_evaluate_current_action() == -1)
                            return -1;
                    }
                    ai->math_multiplier_index++;
                } while (ai->math_multiplier_index < CALCULATOR_ABILITY_COUNT);
            }
            ai_insert_ranked_action();
        }
    }
    ai_move_temp_unit_to_coords(&ai->acting_unit_coords);
    return 0;
}
