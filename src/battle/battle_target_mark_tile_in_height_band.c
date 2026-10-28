#include "fft/battle_map.h"
#include "fft/battle_target.h"
#include "psx/types.h"

/* Put square (x, y) in range (range 1) on each layer whose surface height, in half levels, lies
 * within lo..hi (a negative lo counts as 0); off-map squares are skipped.
 * battle_target_calculate_strike_lunge_range builds its ranges from it. */
void battle_target_mark_tile_in_height_band(s32 x, s32 y, s32 lo, s32 hi) {
    s32 level;
    s32 idx;
    s32 height;
    map_tile_t* tile;
    battle_target_panel_t* panel;
    map_tile_depth_t flags;

    if ((x >= 0) && (x < g_battle_map_max_x) && (y >= 0) && (y < g_battle_map_max_y)) {
        level = 0;
        if (lo < 0) {
            lo = 0;
        }
        do {
            idx = (level << 8) + (y * g_battle_map_max_x) + x;
            tile = &g_battle_map_tile_data[idx];
            panel = &g_battle_target_panels[idx];
            flags = tile->depth_half_height;
            height = (tile->height * 2) + (flags.half_height) + ((flags.depth) * 2);
            level++;
            if ((height >= lo) && (hi >= height)) {
                panel->remaining_range = 1;
            }
        } while (level < 2);
    }
}
