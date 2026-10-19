#include "fft/battle_ai.h"

/*
 * Count candidate tiles within the map bounds on both levels.
 *
 * Each level has 18 rows of 16-bit values, with bit 15 representing x = 0.
 */
s32 battle_ai_count_flagged_tiles_within_map_bounds(u16 rows[MAP_TERRAIN_LAYER_COUNT][BATTLE_AI_GRID_ROW_COUNT]) {
    battle_ai_data_t* ai;
    s32 count;
    s32 level;
    s32 y;
    s32 x;
    u16 row;

    count = 0;
    ai = &g_battle_ai_data_base;
    for (level = 0; level < MAP_TERRAIN_LAYER_COUNT; level++) {
        for (y = 0; y < ai->map_max_y; y++) {
            row = rows[level][y];
            if (row != 0) {
                for (x = 0; x < ai->map_max_x; x++) {
                    if (((row << x) & 0x8000) != 0) {
                        count++;
                    }
                }
            }
        }
    }
    return count;
}
