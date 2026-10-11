#include "fft/battle.h"
#include "psx/types.h"

/* Enter the illegal-move state and show move feedback option 2, which rejects the
 * chosen destination. */
void battle_menu_open_illegal_move_help(void) {
    battle_unit_misc_data_t* unit;
    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_ILLEGAL_MOVE_MENU;
    unit = battle_unit_get_source_misc_data();
    battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_MOVE_FEEDBACK, 2, unit->battle_data->misc_unit_id, 0,
        unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
}
