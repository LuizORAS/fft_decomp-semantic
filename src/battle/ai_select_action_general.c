#include "fft/battle_ai.h"

/*
 * Run or resume the general autobattle action search.
 *
 * Return -1 to suspend and 0 when the selected action is ready. Keeping a
 * charged action skips the new ability search. The switch preserves the
 * target's completion-return delay slot.
 */
s32 ai_select_action_general(void) {
    ai_data_t* ai = &g_ai_data_base;
    s32 charging_result;
    ai_autobattle_stage_e resume_state;

    if (g_ai_data_base.resume_pending != 0) {
        /* Resume: re-enter the step that suspended. */
        resume_state = g_ai_data_base.autobattle_state;
        if (resume_state == AI_AUTOBATTLE_MOVEMENT)
            goto evaluate_movement;
        if (resume_state == AI_AUTOBATTLE_CHARGING)
            goto evaluate_charging;
        goto evaluate_abilities;
    }
    g_ai_data_base.movement_scenario = 1;
evaluate_movement:
    if (ai_build_movement_scenarios() == -1) {
        ai->autobattle_state = AI_AUTOBATTLE_MOVEMENT;
        return -1;
    }
    ai->movement_scenario = 0;
    ai_invert_target_priority();
    ai->movement_scenario = 1;
evaluate_charging:
    charging_result = ai_evaluate_charging_movement();
    switch (charging_result) {
    case -1:
        ai->autobattle_state = AI_AUTOBATTLE_CHARGING;
        return -1;
    case 1:
        return 0;
    }
    ai->movement_scenario = 1;
    if (ai_select_active_units(AI_UNIT_FILTER_ANY, 0) != 0) {
    evaluate_abilities:
        if (ai_evaluate_ability_list() == -1) {
            ai->autobattle_state = AI_AUTOBATTLE_ABILITIES;
            return -1;
        }
    }
    if (ai_select_action_ranked() == 0) {
        ai->movement_scenario = 0;
        ai_select_action_fallback();
    }
    return 0;
}
