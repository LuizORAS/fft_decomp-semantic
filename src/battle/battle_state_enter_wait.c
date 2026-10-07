#include "fft/battle.h"

/* Enter Wait after the unit's command, or what replaces it. When system command 8 reports a due
 * event (option 2 only asks) whose finish operation is nonzero, the state is saved, the event starts
 * and the battle enters EVENT. Otherwise a unit whose turn status blocks the Wait window (CT frozen,
 * incapacitated, dead or asleep) passes the turn (battle_turn_advance), and any other enters
 * WAIT_MENU with its unit panel and the Wait window (system command 3). The map cursor shows for a
 * player-controlled unit and hides for the AI. */
void battle_state_enter_wait(void) {
    battle_unit_misc_data_t* unit;

    battle_state_disable_camera_pan();
    unit = battle_unit_get_source_misc_data();
    if (battle_menu_init_system_function(8, 2, unit->battle_data->misc_unit_id, 0, 1) == 2
        && battle_script_get_event_finish_operation() != 0) {
        g_previous_battle_game_state = g_battle_game_state;
        battle_menu_init_system_function(8, 0, unit->battle_data->misc_unit_id, 0, 1);
        battle_state_enter_event();
        return;
    }
    if (battle_turn_get_status_flags(unit->battle_data) & BATTLE_TURN_STATUS_BLOCKS_WAIT_MENU_MASK) {
        battle_turn_advance();
    } else {
        g_battle_game_state = BATTLE_GAME_STATE_WAIT_MENU;
        battle_target_show_cursor_unit_panel();
        battle_menu_init_system_function(
            3, 0, unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
    }
    if (unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) {
        battle_target_show_cursor();
    } else {
        battle_target_hide_cursor();
    }
}
