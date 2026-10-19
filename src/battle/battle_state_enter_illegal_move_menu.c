#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "fft/battle_menu.h"
#include "fft/battle_unit.h"

/* Enter ILLEGAL_MOVE_MENU and show move feedback option 2, the message that rejects the chosen
 * destination. */
void battle_state_enter_illegal_move_menu(void) {
    battle_unit_t* unit;
    battle_camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_ILLEGAL_MOVE_MENU;
    unit = battle_unit_get_source();
    battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_MOVE_FEEDBACK, 2, unit->stats->battle_id, 0,
        unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
}
