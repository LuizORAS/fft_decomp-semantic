#include "fft/battle_ai.h"
#include "fft/battle_unit.h"

/*
 * Select an action for the recovery-oriented AI policy.
 *
 * Check urgent statuses before the designated-target, enemy and ally passes.
 * Return -1 to suspend and 0 when selection completes.
 */
s32 ai_select_action_recovery(void) {
    ai_data_t* ai = &g_ai_data_base;
    unit_stats_t* acting = g_ai_data_base.acting_unit;
    u8 statuses[AI_RECOVERY_STATUS_COUNT];
    s32 target;
    u32 status;
    s32 result;

    memcpy(statuses, g_ai_recovery_status_ids, sizeof(statuses));
    target = acting->auto_battle_target;
    if (g_ai_data_base.resume_pending) {
        /* Resume: each phase re-enters the step that suspended. */
        switch (g_ai_data_base.action_selection_phase) {
        case AI_RECOVERY_SELECTION_SCORE_TILES:
            goto score_tiles;
        case AI_RECOVERY_SELECTION_MOVEMENT:
            goto movement;
        case AI_RECOVERY_SELECTION_CHARGING:
            goto charging;
        case AI_RECOVERY_SELECTION_TARGET_ABILITIES:
            goto target_abilities;
        case AI_RECOVERY_SELECTION_ENEMY_ABILITIES:
            goto enemy_abilities;
        case AI_RECOVERY_SELECTION_CANCEL_STATUS:
            goto cancel_status;
        default:
            goto ally_abilities;
        }
    }
    g_ai_data_base.movement_scenario = 0;
score_tiles:
    if (ai_score_reachable_tiles() == -1) {
        ai->action_selection_phase = AI_RECOVERY_SELECTION_SCORE_TILES;
        return -1;
    }
    g_ai_recovery_status_retry = 0;
    if (((ai->unit_decisions[target].targeting_flags_2 >> 4) & (AI_TARGET_UNTARGETABLE >> 4))
        || ai->unit_decisions[target].enemy_flag) {
    retarget:
        ai_select_peril_target();
        target = ai->acting_unit->auto_battle_target;
    }
    ai_focus_on_target(target);
movement:
    if (ai_build_targeted_movement() == -1) {
        ai->action_selection_phase = AI_RECOVERY_SELECTION_MOVEMENT;
        return -1;
    }
    ai->movement_scenario = 2;
    ai_invert_target_priority();
    ai->movement_scenario = 0;
    if (!g_ai_recovery_status_retry
        && !((ai->unit_decisions[target].targeting_flags_2 >> 6) & (AI_TARGET_DEAD_WITH_RERAISE >> 6))) {
        g_ai_recovery_status_index = 0;
        do {
            u32 status_flags
                = g_battle_unit_stats[target].status_sets.current[(status = statuses[g_ai_recovery_status_index]) >> 3];
            if (status_flags & (0x80 >> (status & 7))) {
            cancel_status:
                result = ai_evaluate_status_cancellation(target, statuses[g_ai_recovery_status_index]);
                if (result == -1) {
                    ai->action_selection_phase = AI_RECOVERY_SELECTION_CANCEL_STATUS;
                    return -1;
                }
                if (result == 2)
                    return 0;
                if (result == 0) {
                    /* Retry target selection once, then skip further
                     * status-cancel attempts; the retry re-enters the
                     * retarget block above. */
                    g_ai_recovery_status_retry = 1;
                    goto retarget;
                }
            }
            g_ai_recovery_status_index++;
        } while (g_ai_recovery_status_index < AI_RECOVERY_STATUS_COUNT);
    }
    ai->movement_scenario = 1;
charging:
    result = ai_evaluate_charging_movement();
    switch (result) {
    case -1:
        ai->action_selection_phase = AI_RECOVERY_SELECTION_CHARGING;
        return -1;
    case 1:
        return 0;
    }
    ai->movement_scenario = 0;
    if (ai_select_active_units(AI_UNIT_FILTER_SPECIFIC, target)) {
    target_abilities:
        if (ai_evaluate_ability_list() == -1) {
            ai->action_selection_phase = AI_RECOVERY_SELECTION_TARGET_ABILITIES;
            return -1;
        }
        if (ai_select_action_ranked())
            return 0;
    }
    ai->movement_scenario = 1;
    if (ai_select_active_units(AI_UNIT_FILTER_ENEMIES, 0)) {
    enemy_abilities:
        if (ai_evaluate_ability_list() == -1) {
            ai->action_selection_phase = AI_RECOVERY_SELECTION_ENEMY_ABILITIES;
            return -1;
        }
        if (ai_select_action_ranked())
            return 0;
    }
    if (ai_select_active_units(AI_UNIT_FILTER_ALLIES, 0)) {
    ally_abilities:
        if (ai_evaluate_ability_list() != -1) {
            if (ai_select_action_ranked())
                return 0;
        } else {
            ai->action_selection_phase = AI_RECOVERY_SELECTION_ALLY_ABILITIES;
            return -1;
        }
    }
    ai->movement_scenario = 2;
    ai_select_action_fallback();
    return 0;
}
