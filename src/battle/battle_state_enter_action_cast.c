#include "fft/battle.h"

/* Enter the action-cast state for the source unit, unless the scenario check
 * starts an event first; the camera turns when the unit's tile is hidden. */
void battle_state_enter_action_cast(void) {
    battle_unit_misc_data_t* unit;

    battle_state_stop_game_flow();
    g_battle_game_state = BATTLE_GAME_STATE_ACTION_CAST;
    g_action_post_action = 0;
    unit = battle_unit_get_source_misc_data();
    if (battle_state_try_enter_scenario_event(unit) == 0) {
        g_battle_game_state = BATTLE_GAME_STATE_ACTION_CAST;
        camera_rotate_when_unit_tile_not_visible(unit);
    }
}
