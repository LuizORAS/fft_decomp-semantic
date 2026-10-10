#include "fft/battle.h"

/* Enter the move-range-exception state, reached when the casting unit has no
 * reachable tile: opens targeting-prompt option 2 for the source unit. */
void battle_state_enter_move_range_exception(void) {
    battle_unit_misc_data_t* unit;

    battle_state_stop_game_flow();
    g_battle_game_state = BATTLE_GAME_STATE_MOVE_RANGE_EXCEPTION;
    unit = battle_unit_get_source_misc_data();
    battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_TARGETING_PROMPT, 2, unit->battle_data->misc_unit_id, 0,
        unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
}
