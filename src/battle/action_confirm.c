#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_menu.h"

/* Enter the confirm-action state and open the confirmation prompt for the pending
 * action.
 *
 * The prompt option follows the ability preview's targeting mode and whether a
 * selectable unit stands on the cursor tile: 1 for a unit under a unit-or-tile
 * ability, 4 for a unit under a tile-only ability, 3 for a unit under an ability
 * with no target choice, 2 for an empty tile in all three, and 0 for any other
 * mode. */
void action_confirm(void) {
    battle_unit_misc_data_t* unit;
    battle_unit_misc_data_t* target;

    battle_state_stop_game_flow();
    g_battle_game_state = BATTLE_GAME_STATE_CONFIRM_ACTION;
    unit = battle_unit_get_source_misc_data();
    target
        = battle_unit_get_selectable_misc_data_at_map_coords(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
    switch (unit->ability_preview_phase) {
    case TARGET_PANELS_UNIT_OR_TILE:
        if (target != 0) {
            battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_ACTION_CONFIRM, 1,
                unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        } else {
            battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_ACTION_CONFIRM, 2,
                unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        }
        break;
    case TARGET_PANELS_TILE_ONLY:
        if (target != 0) {
            battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_ACTION_CONFIRM, 4,
                unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        } else {
            battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_ACTION_CONFIRM, 2,
                unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        }
        break;
    case TARGET_PANELS_NO_TARGETING:
        if (target != 0) {
            battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_ACTION_CONFIRM, 3,
                unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        } else {
            battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_ACTION_CONFIRM, 2,
                unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        }
        break;
    default:
        battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_ACTION_CONFIRM, 0, unit->battle_data->misc_unit_id,
            0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        break;
    }
}
