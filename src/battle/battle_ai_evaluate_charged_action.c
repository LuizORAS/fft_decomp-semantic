#include "fft/battle_ai.h"
#include "fft/memory.h"

/* Decide whether a unit with a charged action keeps it: returns 1 when keeping it scores above
 * dropping it, 0 to choose anew, -1 to suspend. A unit without one returns 0. A Charge (Archer) is
 * judged where the unit stands, any other at the scenario's destination with its crystal or treasure.
 * A Sing or Dance performance only enters the ranking (battle_ai_score_current_action,
 * battle_ai_insert_ranked_action) and returns 0. Otherwise the state is scored with the charge and
 * again with the charge, Charging and Performing removed; when keeping wins, the selected action is
 * to move without a new ability.
 *
 * The second simulation temporarily changes unit data; a suspended pass keeps those changes until it
 * resumes and completes. */
s32 battle_ai_evaluate_charged_action(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    battle_stats_t* unit = g_battle_ai_data_base.acting_unit;
    action_command_t* action = &unit->action_command;
    battle_ai_charging_stage_e state;

    if (g_battle_ai_data_base.resume_pending) {
        /* Resume: re-enter the step that suspended. */
        state = g_battle_ai_data_base.charging_state;
        if (state == BATTLE_AI_CHARGING_SONG_DANCE)
            goto evaluate_song_dance;
        if (state == BATTLE_AI_CHARGING_WITH_CHARGE)
            goto evaluate_with_charge;
        goto evaluate_without_charge;
    }
    if (unit->charged_ability_ct == 0xff)
        return 0;
    if (g_main_action_menu_types_by_skillset[unit->action_command.skillset_id] == ACTION_MENU_TYPE_CHARGE) {
        g_battle_ai_data_base.current_action.coords = g_battle_ai_data_base.acting_unit_coords;
    } else {
        g_battle_ai_data_base.current_action.coords = ai->candidate_coords[g_battle_ai_data_base.movement_scenario];
        battle_ai_update_tile_pickup();
    }
    ai->current_action.act_before_move = 0;
    ai->current_action.command.skillset_id = 0;
    if (action->ability_id >= ABILITY_ID_SONG_ANGEL_SONG && action->ability_id <= ABILITY_ID_DANCE_LAST_DANCE) {
    evaluate_song_dance:
        if (battle_ai_score_current_action() == -1) {
            ai->charging_state = BATTLE_AI_CHARGING_SONG_DANCE;
            return -1;
        }
        battle_ai_insert_ranked_action();
        return 0;
    }
evaluate_with_charge:
    if (battle_ai_simulate_action_and_score() == -1) {
        ai->charging_state = BATTLE_AI_CHARGING_WITH_CHARGE;
        return -1;
    }
    battle_mem_copy_u16_values(&ai->selected_action, &ai->current_action, sizeof(battle_ai_action_data_t));
    ai->saved_ability_ct = ai->acting_unit->charged_ability_ct;
    ai->saved_current_status = ai->acting_unit->status_sets.current[0];
    ai->saved_inflicted_status = ai->acting_unit->inflicted_status[0];
    ai->acting_unit->charged_ability_ct = 0xff;
    ai->acting_unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHARGING)]
        &= ~(BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARGING) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PERFORMING));
    ai->acting_unit->inflicted_status[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHARGING)]
        &= ~(BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARGING) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PERFORMING));
evaluate_without_charge:
    if (battle_ai_simulate_action_and_score() != -1) {
        ai->acting_unit->charged_ability_ct = ai->saved_ability_ct;
        ai->acting_unit->status_sets.current[0] = ai->saved_current_status;
        ai->acting_unit->inflicted_status[0] = ai->saved_inflicted_status;
        return battle_ai_compare_action_rank(&ai->selected_action.rank.decided, &ai->current_action.rank.decided)
            == BATTLE_AI_RANK_ABOVE;
    }
    ai->charging_state = BATTLE_AI_CHARGING_WITHOUT_CHARGE;
    return -1;
}
