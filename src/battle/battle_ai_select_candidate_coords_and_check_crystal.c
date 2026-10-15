#include "fft/battle.h"

/* Turn the origin in the current action's coords into a plan: an origin on the unit's own tile acts
 * first and then moves to the scenario's destination, any other moves there and then acts. Then note
 * the crystal or treasure at the destination
 * (battle_ai_check_unit_for_crystal_or_treasure_status). */
void battle_ai_select_candidate_coords_and_check_crystal(void) {
    battle_ai_data_t* ai;

    ai = &g_battle_ai_data_base;
    if (g_battle_ai_data_base.current_action.coords.word != g_battle_ai_data_base.acting_unit_coords.word) {
        g_battle_ai_data_base.current_action.act_before_move = 0;
    } else {
        g_battle_ai_data_base.current_action.act_before_move = 1;
        g_battle_ai_data_base.current_action.coords.word
            = ai->candidate_coords[g_battle_ai_data_base.movement_scenario].word;
    }
    battle_ai_check_unit_for_crystal_or_treasure_status();
}
