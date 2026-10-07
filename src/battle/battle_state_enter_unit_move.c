#include "fft/battle.h"

/* Enter UNIT_MOVE and open the Move window (system command 1, option 0) for the source unit. The
 * action menus and the AI command call it for the Move command. */
void battle_state_enter_unit_move(void) {
    battle_unit_misc_data_t* unit;

    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_UNIT_MOVE;
    unit = battle_unit_get_source_misc_data();
    battle_menu_init_system_function(
        1, 0, unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
}
