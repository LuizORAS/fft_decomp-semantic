#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "psx/types.h"

/* Enter UNIT_MOVING_SETUP and open the window that precedes the casting unit's move, addressed
 * to the source unit: move feedback 1 for a pending mount; for a teleport path (count
 * BATTLE_MOVE_PATH_TELEPORT or _FAILED) the move confirmation when navigation messages are on,
 * else feedback 0; for a walked path feedback 4 when a crystal lies on the cursor tile, else 0. */
void battle_state_enter_unit_moving_setup(void) {
    battle_unit_t* src;
    battle_unit_t* cast;
    battle_unit_t* at_tile;
    u8 path_count;

    battle_camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_UNIT_MOVING_SETUP;
    src = battle_unit_get_source();
    cast = battle_unit_get_casting();

    if (cast->path.pending_mount & BATTLE_PENDING_MOUNT_SET) {
        battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_MOVE_FEEDBACK, 1, src->stats->battle_id, 0,
            src->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        return;
    }
    path_count = cast->path.count + 2;
    if (path_count < 2) {
        if (g_main_game_options.fields.navigation_messages != GAME_OPTION_ON) {
            battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_MOVE_FEEDBACK, 0, src->stats->battle_id, 0,
                src->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
            return;
        }
        battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_MOVE_CONFIRM, 0, src->stats->battle_id, 0,
            src->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        return;
    }
    at_tile
        = battle_unit_get_crystal_or_treasure_at_map_coords(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
    if (at_tile != 0) {
        if (at_tile->status_flags_5_6 & BATTLE_UNIT_STATUS_CRYSTAL) {
            battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_MOVE_FEEDBACK, 4, src->stats->battle_id, 0,
                src->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
            return;
        }
    }
    battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_MOVE_FEEDBACK, 0, src->stats->battle_id, 0,
        src->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
}
