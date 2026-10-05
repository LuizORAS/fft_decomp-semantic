#include "fft/battle.h"

/* Enter CONFIRM_ACTION and open the "Execute?" window (system command 4). The option follows the
 * range result in ability_preview_phase and the unit under the cursor: with a selectable unit there,
 * 1 for result 0, 4 for result 1 (cannot follow target) and 3 for result 2; 2 with no unit; 0 for
 * any other result. battle_menu_dispatch_system_function maps them to the window variants. */
void battle_state_enter_confirm_action(void) {
    battle_unit_misc_data_t* unit;
    battle_unit_misc_data_t* target;
    s32 mode;

    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_CONFIRM_ACTION;
    unit = battle_unit_get_source_misc_data();
    target
        = battle_unit_get_selectable_misc_data_at_map_coords(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
    switch (unit->ability_preview_phase) {
    case 0:
        if (target != 0) {
            battle_menu_init_system_function(
                4, 1, unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        } else {
            battle_menu_init_system_function(
                4, 2, unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        }
        break;
    case 1:
        if (target != 0) {
            battle_menu_init_system_function(
                4, 4, unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        } else {
            battle_menu_init_system_function(
                4, 2, unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        }
        break;
    case 2:
        if (target != 0) {
            battle_menu_init_system_function(
                4, 3, unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        } else {
            battle_menu_init_system_function(
                4, 2, unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        }
        break;
    default:
        battle_menu_init_system_function(
            4, 0, unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        break;
    }
}
