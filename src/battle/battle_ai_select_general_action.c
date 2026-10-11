#include "fft/battle.h"

/* The general search, for a unit without a controlling status or an Auto-Battle mode: build the
 * movement scenarios, take moving to the destination as the baseline (battle_ai_set_movement_baseline),
 * keep a charged action when that scores better (battle_ai_evaluate_charged_action), then rank every
 * usable ability against every unit worth targeting (battle_ai_evaluate_ability_list) and take a ranked
 * action (battle_ai_select_ranked_action); with none, it only moves (and Defends when Defend is its
 * support ability). Returns -1 to suspend, else 0. */
s32 battle_ai_select_general_action(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 charging_result;
    s32 resume_state;

    if (g_battle_ai_data_base.resume_pending != 0) {
        /* Resume: re-enter the step that suspended. */
        resume_state = g_battle_ai_data_base.autobattle_state;
        if (resume_state == 4)
            goto evaluate_movement;
        if (resume_state == 2)
            goto evaluate_charging;
        goto evaluate_abilities;
    }
    g_battle_ai_data_base.movement_scenario = 1;
evaluate_movement:
    if (battle_ai_build_movement_scenarios() == -1) {
        ai->autobattle_state = 4;
        return -1;
    }
    ai->movement_scenario = 0;
    battle_ai_set_movement_baseline();
    ai->movement_scenario = 1;
evaluate_charging:
    charging_result = battle_ai_evaluate_charged_action();
    switch (charging_result) {
    case -1:
        ai->autobattle_state = 2;
        return -1;
    case 1:
        return 0;
    }
    ai->movement_scenario = 1;
    if (battle_ai_select_active_units(BATTLE_AI_UNIT_FILTER_ANY, 0) != 0) {
    evaluate_abilities:
        if (battle_ai_evaluate_ability_list() == -1) {
            ai->autobattle_state = 7;
            return -1;
        }
    }
    if (battle_ai_select_ranked_action() == 0) {
        ai->movement_scenario = 0;
        battle_ai_select_move_only_action();
    }
    return 0;
}
