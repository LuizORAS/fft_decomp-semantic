#include "fft/battle.h"
#include "psx/types.h"

/* Leave only the action's target tile marked as targeted. Returns 1, or -1
 * when that tile is off the map or blocked. */
s32 battle_target_disable_green_panel_on_all_but_target_tile(const battle_ai_command_action_t* action) {
    battle_ai_command_action_t action_copy;
    u8 x;
    u8 y;
    u8 level;
    map_tile_t* tile;

    main_util_copy_action_data(action, &action_copy);
    /* Retail reads only the low byte of each coordinate halfword. */
    x = *(u8*)&action_copy.target_x;
    y = *(u8*)&action_copy.target_y;
    level = *(u8*)&action_copy.target_elevation;
    if (x < g_battle_map_max_x) {
        if (y < g_battle_map_max_y) {
            if (level < 2) {
                tile = &g_battle_map_tile_data[(level << 8) + y * g_battle_map_max_x + x];
                if ((tile->flags.value & MAP_TILE_FLAG_BLOCKED) == 0) {
                    battle_target_disable_green_panel_flags();
                    tile->ceiling_depth_and_marks.value |= MAP_TILE_FLAG_TARGETED;
                    return 1;
                }
            }
        }
    }
    return -1;
}
