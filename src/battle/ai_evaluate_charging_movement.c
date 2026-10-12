#include "fft/battle_ai.h"

/* Evaluate movement while keeping or removing the acting unit's charge.
 *
 * The second simulation temporarily changes unit data; a suspended pass
 * preserves those changes until resumed and completed. The simulation helper
 * resumes from its saved phase when the frame budget is exhausted.
 */
s32 ai_evaluate_charging_movement(void) {
    ai_data_t* ai = &g_ai_data_base;
    battle_stats_t* unit = g_ai_data_base.acting_unit;
    action_command_t* action = &unit->action_command;
    ai_charging_stage_e state;

    if (g_ai_data_base.resume_pending) {
        /* Resume: re-enter the step that suspended. */
        state = g_ai_data_base.charging_state;
        if (state == AI_CHARGING_SONG_DANCE)
            goto evaluate_song_dance;
        if (state == AI_CHARGING_WITH_CHARGE)
            goto evaluate_with_charge;
        goto evaluate_without_charge;
    }
    if (unit->charged_ability_ct == 0xff)
        return 0;
    if (g_main_action_menu_types_by_skillset[unit->action_command.skillset_id] == ACTION_MENU_TYPE_CHARGE) {
        g_ai_data_base.current_action.coords = g_ai_data_base.acting_unit_coords;
    } else {
        g_ai_data_base.current_action.coords = ai->candidate_coords[g_ai_data_base.movement_scenario];
        ai_check_unit_for_crystal_or_treasure_status();
    }
    ai->current_action.act_before_move = 0;
    ai->current_action.command.skillset_id = 0;
    if (action->ability_id >= ABILITY_ID_SONG_ANGEL_SONG && action->ability_id <= ABILITY_ID_DANCE_LAST_DANCE) {
    evaluate_song_dance:
        if (ai_check_set_highest_unit_priority() == -1) {
            ai->charging_state = AI_CHARGING_SONG_DANCE;
            return -1;
        }
        ai_insert_ranked_action();
        return 0;
    }
evaluate_with_charge:
    if (ai_simulate_action_and_score() == -1) {
        ai->charging_state = AI_CHARGING_WITH_CHARGE;
        return -1;
    }
    ai_transfer_halfword_values(&ai->selected_action, &ai->current_action, sizeof(ai_action_data_t));
    ai->saved_ability_ct = ai->acting_unit->charged_ability_ct;
    ai->saved_current_status = ai->acting_unit->status_sets.current[0];
    ai->saved_inflicted_status = ai->acting_unit->inflicted_status[0];
    ai->acting_unit->charged_ability_ct = 0xff;
    ai->acting_unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHARGING)]
        &= ~(BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARGING) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PERFORMING));
    ai->acting_unit->inflicted_status[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHARGING)]
        &= ~(BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARGING) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PERFORMING));
evaluate_without_charge:
    if (ai_simulate_action_and_score() != -1) {
        ai->acting_unit->charged_ability_ct = ai->saved_ability_ct;
        ai->acting_unit->status_sets.current[0] = ai->saved_current_status;
        ai->acting_unit->inflicted_status[0] = ai->saved_inflicted_status;
        return ai_compare_target_priority_and_hit_rate(&ai->selected_action.rank, &ai->current_action.rank)
            == AI_RANK_ABOVE;
    }
    ai->charging_state = AI_CHARGING_WITHOUT_CHARGE;
    return -1;
}
