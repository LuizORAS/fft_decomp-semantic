#include "fft/battle_ai.h"

/* Turn the origin in the current action's coords into a plan: an origin on the unit's own tile acts
 * first and then moves to the scenario's destination, any other moves there and then acts. Then note
 * the crystal or treasure at the destination
 * (battle_ai_update_tile_pickup). */
void battle_ai_finalize_action_destination(void) {
    battle_ai_data_t* ai;

    ai = &g_battle_ai_data_base;
    if (g_battle_ai_data_base.current_action.coords.word != g_battle_ai_data_base.acting_unit_coords.word) {
        g_battle_ai_data_base.current_action.act_before_move = 0;
    } else {
        g_battle_ai_data_base.current_action.act_before_move = 1;
        g_battle_ai_data_base.current_action.coords = ai->candidate_coords[g_battle_ai_data_base.movement_scenario];
    }
    battle_ai_update_tile_pickup();
}
