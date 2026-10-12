#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_menu.h"

/* Enter the unit-move state and show the source unit's targeting prompt. */
void battle_action_handle_move_command(void) {
    battle_unit_misc_data_t* unit;

    battle_state_stop_game_flow();
    g_battle_game_state = BATTLE_GAME_STATE_UNIT_MOVE;
    unit = battle_unit_get_source_misc_data();
    battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_TARGETING_PROMPT, 0, unit->battle_data->misc_unit_id, 0,
        unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
}
