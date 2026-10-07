#include "fft/battle.h"

/* Enter MOVE_RANGE_EXCEPTION when the mover has no tile to reach (battle_menu_close_move_help):
 * system command 1 with option 2 opens a message in place of the destination choice (system function
 * 0x31 for the Move command). */
void battle_state_enter_move_range_exception(void) {
    battle_unit_misc_data_t* unit;

    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_MOVE_RANGE_EXCEPTION;
    unit = battle_unit_get_source_misc_data();
    battle_menu_init_system_function(
        1, 2, unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
}
