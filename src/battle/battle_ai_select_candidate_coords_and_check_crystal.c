#include "fft/battle_ai.h"

/*
 * Mark an actor-origin choice and check the resulting tile for crystal or treasure.
 *
 * An actor-origin choice sets act_before_move and substitutes the movement
 * scenario's candidate coordinates; other origins clear that flag.
 */
void battle_ai_select_candidate_coords_and_check_crystal(void) {
    battle_ai_data_t* ai;

    ai = &g_battle_ai_data_base;
    if (g_battle_ai_data_base.current_action.coords.word != g_battle_ai_data_base.acting_unit_coords.word) {
        g_battle_ai_data_base.current_action.act_before_move = 0;
    } else {
        g_battle_ai_data_base.current_action.act_before_move = 1;
        g_battle_ai_data_base.current_action.coords = ai->candidate_coords[g_battle_ai_data_base.movement_scenario];
    }
    battle_ai_check_unit_for_crystal_or_treasure_status();
}
