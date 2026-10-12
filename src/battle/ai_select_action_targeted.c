#include "fft/battle_ai.h"
#include "fft/battle_unit.h"

/*
 * Select an AI action around the unit's designated target.
 *
 * Preserve the target across the staged movement and ability passes. Return
 * -1 to suspend and 0 when selection completes.
 */
s32 ai_select_action_targeted(void) {
    s32 target = g_ai_data_base.acting_unit->auto_battle_target;
    ai_data_t* ai = &g_ai_data_base;
    battle_stats_t* unit;
    s32 i, result;

    /* Reload the persistent target before every resume dispatch. */
    if (g_ai_data_base.resume_pending) {
        switch (g_ai_data_base.action_selection_phase) {
        case AI_TARGETED_SELECTION_SCORE_TILES:
            goto score_tiles;
        case AI_TARGETED_SELECTION_MOVEMENT:
            goto movement;
        case AI_TARGETED_SELECTION_CHARGING:
            goto charging;
        case AI_TARGETED_SELECTION_TARGET_ABILITIES:
            goto target_abilities;
        default:
            goto other_abilities;
        }
    }
    if (ai_check_target_type(target) || !ai->unit_decisions[target].enemy_flag) {
        g_ai_data_base.acting_unit_decision->flags &= ~AI_DECISION_SPECIAL_BEHAVIOR;
        unit = &g_battle_unit_stats[target];
        if (unit->mount_info & BATTLE_MOUNT_INFO_FLAG_MOUNT) {
            if (!((unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_TRANSPARENT)]
                      & STATUS_BYTE_MASK(STATUS_ID_TRANSPARENT))
                    && (g_battle_unit_stats[unit->mount_info & BATTLE_MOUNT_INFO_PARTNER_ID_MASK]
                            .status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_TRANSPARENT)]
                        & STATUS_BYTE_MASK(STATUS_ID_TRANSPARENT))))
                target = unit->mount_info & BATTLE_MOUNT_INFO_PARTNER_ID_MASK;
        } else {
            /* A direct member pointer folds the target's base cursor and field
             * displacement together. Derive the member offset to retain its stores. */
            u32 cursor;
            s32 included = 1;
            i = BATTLE_UNIT_SLOT_COUNT - 1;
            cursor = (u32)ai + i;
            do {
                *(u8*)(cursor + ((u32)&ai->targetability.live.unit_targetable - (u32)ai)) = included;
                i--;
                cursor--;
            } while (i >= 0);
            target = ai_find_nearest_target(AI_NEAREST_ELIGIBLE_ENEMY);
            if (target == ai->acting_unit_id) {
                for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
                    unit = &g_battle_unit_stats[i];
                    if (unit->entd_slot != BATTLE_ENTD_SLOT_NONE && ai->unit_decisions[i].enemy_flag
                        && (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_TRANSPARENT)]
                            & STATUS_BYTE_MASK(STATUS_ID_TRANSPARENT))) {
                        target = i;
                        break;
                    }
                }
            }
        }
    }
    ai->acting_unit->auto_battle_target = target;
    ai_focus_on_target(target);
    ai->movement_scenario = 0;
score_tiles:
    if (ai_score_reachable_tiles() == -1) {
        ai->action_selection_phase = AI_TARGETED_SELECTION_SCORE_TILES;
        return -1;
    }
movement:
    if (ai_build_targeted_movement() == -1) {
        ai->action_selection_phase = AI_TARGETED_SELECTION_MOVEMENT;
        return -1;
    }
    ai->movement_scenario = 2;
    ai_invert_target_priority();
    ai->movement_scenario = 0;
charging:
    result = ai_evaluate_charging_movement();
    switch (result) {
    case -1:
        ai->action_selection_phase = AI_TARGETED_SELECTION_CHARGING;
        return -1;
    case 1:
        return 0;
    }
    ai->movement_scenario = 0;
    if (ai_select_active_units(AI_UNIT_FILTER_SPECIFIC, target)) {
    target_abilities:
        if (ai_evaluate_ability_list() == -1) {
            ai->action_selection_phase = AI_TARGETED_SELECTION_TARGET_ABILITIES;
            return -1;
        }
        if (ai_select_action_ranked())
            return 0;
    }
    ai->movement_scenario = 1;
    if (ai_select_active_units(AI_UNIT_FILTER_ANY, 0)) {
    other_abilities:
        if (ai_evaluate_ability_list() == -1) {
            ai->action_selection_phase = AI_TARGETED_SELECTION_OTHER_ABILITIES;
            return -1;
        }
        if (ai_select_action_ranked())
            return 0;
    }
    ai->movement_scenario = 2;
    ai_select_action_fallback();
    return 0;
}
