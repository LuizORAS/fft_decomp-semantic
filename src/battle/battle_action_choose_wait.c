#include "fft/battle.h"

void battle_action_choose_wait(void) {
    battle_unit_misc_data_t* unit;

    battle_state_disable_camera_pan();
    unit = battle_unit_get_source_misc_data();
    if (battle_menu_init_system_function(8, 2, unit->battle_data->misc_unit_id, 0, 1) == 2
        && battle_script_get_event_finish_operation() != 0) {
        g_previous_battle_game_state = g_battle_game_state;
        battle_menu_init_system_function(8, 0, unit->battle_data->misc_unit_id, 0, 1);
        battle_action_set_casting_unit_id_ff();
        return;
    }
    if (battle_turn_get_status_flags(unit->battle_data) & BATTLE_TURN_STATUS_BLOCKS_WAIT_MENU_MASK) {
        battle_turn_advance();
    } else {
        g_battle_game_state = BATTLE_GAME_STATE_WAIT_MENU;
        battle_target_store_cursor_unit_name_and_data();
        battle_menu_init_system_function(
            3, 0, unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
    }
    if (unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) {
        battle_target_show_cursor();
    } else {
        battle_target_hide_cursor();
    }
}
