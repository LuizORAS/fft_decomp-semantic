#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_camera.h"
#include "fft/battle_menu.h"

/* Enter UNIT_MOVE and open the Move window (system command 1, option 0) for the source unit. The
 * action menus and the AI command call it for the Move command. */
void battle_state_enter_unit_move(void) {
    battle_unit_t* unit;

    battle_camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_UNIT_MOVE;
    unit = battle_unit_get_source();
    battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_TARGETING_PROMPT, 0, unit->stats->battle_id, 0,
        unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
}
