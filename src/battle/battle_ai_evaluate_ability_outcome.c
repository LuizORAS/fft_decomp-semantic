#include "fft/battle.h"

/* Evaluate the uses of the considered ability for battle_ai_evaluate_ability_list and rank the best
 * one (battle_ai_insert_ranked_action). An entry the AI may not use is skipped, and a random-use
 * ability is tried only when rand() % 128 is at most random_use_threshold. A map-tile ability targets
 * the unit itself at the scenario's destination: Math Skill goes through
 * battle_ai_evaluate_math_targets, any other is judged before and after moving
 * (battle_ai_evaluate_before_and_after_move) when it is useful on some unit. Otherwise the search
 * tries a bounce off a Reflect unit for a reflectable ability
 * (battle_ai_evaluate_reflected_target_origins), keeps the units the ability is useful on
 * (battle_ai_has_any_unit_decided_to_use_ability; none ends it), tries areas around the unit itself
 * (battle_ai_evaluate_self_target_origins), then searches the origins (battle_ai_evaluate_origins).
 * A follow-target ability is aimed at units: without CONSUME_TARGET_TILES, first at the units that
 * cannot move before a charge lands (battle_ai_check_ability_use_based_on_ct), dropping evaluated
 * tiles, then at the rest. Except for a follow-target ability without area, it is then aimed at
 * tiles once the enemies that can move first are left out
 * (battle_ai_exclude_enemies_acting_before_ct). Returns -1 to suspend (also past 441 horizontal
 * blanks), else 0. */
s32 battle_ai_evaluate_ability_outcome(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 i;
    if (g_battle_ai_data_base.decision_state) {
        /* Resume: each state re-enters the evaluation step that suspended. */
        switch (g_battle_ai_outcome_evaluation_state) {
        case 0:
            goto evaluate_reflected_targets;
        case 1:
            goto evaluate_self_target_range;
        case 2:
            goto evaluate_single_unit_pass;
        case 3:
            goto evaluate_first_ct_group;
        case 4:
            goto evaluate_second_ct_group;
        case 6:
            goto evaluate_math_targets;
        case 7:
            break;
        case 8:
            goto evaluate_map_targets;
        default:
            goto evaluate_tile_targets;
        }
    }
    if (VSync(1) >= 441) {
        ai->outcome_evaluation_state = 7;
        return -1;
    }
    {
        s32 flags = ai->considered_ability.ai_flags.word;
        ai->decision_state = 0;
        if (flags >= 0)
            return 0;
        if (flags & BATTLE_AI_ABILITY_RANDOM_USE) {
            i = ai->random_use_threshold;
            if (i < rand() % 128)
                return 0;
        }
    }
    ai->current_action.skillset = ai->considered_ability.skillset;
    ai->current_action.ability_id = ai->considered_ability.ability_id;
    ai->current_action.item_id = ai->considered_ability.parameters.bytes.item_id;
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++)
        ai->targetability.live.unit_targetable[i] = ai->targetability.live.unit_active[i];
    if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_TARGET_MAP_TILES) {
        s32 acting_id = ai->acting_unit_id;
        ai->current_action.targeting_type = BATTLE_ACTION_TARGET_UNIT;
        ai->current_action.target_id = acting_id;
        ai->current_action.coords = ai->candidate_coords[ai->movement_scenario];
        battle_ai_check_unit_for_crystal_or_treasure_status();
        if (g_main_action_menu_types_by_skillset[ai->considered_ability.skillset] == ACTION_MENU_TYPE_ARITHMETICKS) {
        evaluate_math_targets:
            if (battle_ai_evaluate_math_targets() == -1) {
                ai->outcome_evaluation_state = 6;
                return -1;
            }
            return 0;
        }
        if (!battle_ai_has_any_unit_decided_to_use_ability())
            return 0;
    evaluate_map_targets:
        if (battle_ai_evaluate_before_and_after_move() == -1) {
            ai->outcome_evaluation_state = 8;
            return -1;
        }
        battle_ai_insert_ranked_action();
        return 0;
    }
    if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_REFLECTABLE) {
    evaluate_reflected_targets:
        if (battle_ai_evaluate_reflected_target_origins() == -1) {
            ai->outcome_evaluation_state = 0;
            return -1;
        }
    }
    if (!battle_ai_has_any_unit_decided_to_use_ability())
        return 0;
    if (ai->targetability.live.unit_targetable[ai->acting_unit_id]) {
    evaluate_self_target_range:
        if (battle_ai_evaluate_self_target_origins() == -1) {
            ai->outcome_evaluation_state = 1;
            return -1;
        }
    }
    ai->ability_effect_on_self = battle_ai_classify_ability_effect(ai->acting_unit_id, 1);
    if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_FOLLOW_TARGET) {
        ai->current_action.targeting_type = BATTLE_ACTION_TARGET_UNIT;
        if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_CONSUME_TARGET_TILES) {
            ai->targetability.live.target_setting_flags |= BATTLE_AI_TARGET_SETTING_CONSUME_TARGET_TILES;
        evaluate_single_unit_pass:
            if (battle_ai_evaluate_origins() == -1) {
                ai->outcome_evaluation_state = 2;
                return -1;
            }
        } else {
            /* Evaluate complementary CT-filtered target groups, then restore the
             * original eligibility list. The overlapping flag byte stays intact. */
            for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
                ai->targetability.snapshot.unit_targetable_saved[i] = ai->targetability.live.unit_targetable[i];
                if (ai->targetability.live.unit_targetable[i] && !battle_ai_check_ability_use_based_on_ct(i))
                    ai->targetability.live.unit_targetable[i] = 0;
            }
            ai->targetability.live.target_setting_flags |= BATTLE_AI_TARGET_SETTING_CONSUME_TARGET_TILES;
        evaluate_first_ct_group:
            if (battle_ai_evaluate_origins() == -1) {
                ai->outcome_evaluation_state = 3;
                return -1;
            }
            for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++)
                if (ai->targetability.snapshot.unit_targetable_saved[i]) {
                    if (battle_ai_check_ability_use_based_on_ct(i))
                        ai->targetability.live.unit_targetable[i] = 0;
                    else
                        ai->targetability.live.unit_targetable[i] = 1;
                }
            ai->targetability.live.target_setting_flags &= ~BATTLE_AI_TARGET_SETTING_CONSUME_TARGET_TILES;
        evaluate_second_ct_group:
            if (battle_ai_evaluate_origins() == -1) {
                ai->outcome_evaluation_state = 4;
                return -1;
            }
            for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++)
                ai->targetability.live.unit_targetable[i] = ai->targetability.snapshot.unit_targetable_saved[i];
        }
        if (!ai->considered_ability.parameters.bytes.aoe) {
            battle_ai_insert_ranked_action();
            return 0;
        }
    }
    if (battle_ai_exclude_enemies_acting_before_ct()) {
        ai->current_action.targeting_type = BATTLE_ACTION_TARGET_TILE;
        if (ai->considered_ability.ai_flags.word & BATTLE_AI_ABILITY_CONSUME_TARGET_TILES)
            ai->targetability.live.target_setting_flags |= BATTLE_AI_TARGET_SETTING_CONSUME_TARGET_TILES;
    evaluate_tile_targets:
        if (battle_ai_evaluate_origins() == -1) {
            ai->outcome_evaluation_state = 5;
            return -1;
        }
    }
    battle_ai_insert_ranked_action();
    return 0;
}
