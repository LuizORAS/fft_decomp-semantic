#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_camera.h"
#include "fft/battle_menu.h"

/* Enter MOVE_RANGE_EXCEPTION when the mover has no tile to reach (battle_state_enter_select_move_tile):
 * system command 1 with option 2 opens a message in place of the destination choice (system function
 * 0x31 for the Move command). */
void battle_state_enter_move_range_exception(void) {
    battle_unit_t* unit;

    battle_camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_MOVE_RANGE_EXCEPTION;
    unit = battle_unit_get_source();
    battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_TARGETING_PROMPT, 2, unit->stats->battle_id, 0,
        unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
}
