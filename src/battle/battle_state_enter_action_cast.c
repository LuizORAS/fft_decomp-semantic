#include "fft/battle.h"
#include "fft/battle_camera.h"

/* Enter the action-cast state for the source unit, unless the scenario check
 * starts an event first; the camera turns when the unit's tile is hidden. */
void battle_state_enter_action_cast(void) {
    battle_unit_t* unit;

    camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_ACTION_CAST;
    g_action_post_action = 0;
    unit = battle_unit_get_source();
    if (battle_state_try_enter_scenario_event(unit) == 0) {
        g_battle_game_state = BATTLE_GAME_STATE_ACTION_CAST;
        camera_rotate_when_unit_tile_not_visible(unit);
    }
}
