#include "fft/battle_ai.h"

/*
 * Remove a randomly selected tile from a candidate mask.
 *
 * The supplied count determines the random ordinal. Return 0 when it is 0
 * or no corresponding set bit is found; otherwise write coordinates and return 1.
 */
s32 battle_ai_take_random_tile(
    u16 tiles[MAP_TERRAIN_LAYER_COUNT][BATTLE_AI_GRID_ROW_COUNT], battle_ai_coords_t* coordinates, s32 count) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 selected;
    s32 level, y, x;
    u16 row;

    if (count == 0) {
        return 0;
    }
    selected = rand() % count;
    for (level = 0; level < MAP_TERRAIN_LAYER_COUNT; level++) {
        for (y = 0; y < ai->map_max_y; y++) {
            row = tiles[level][y];
            if (row != 0) {
                for (x = 0; x < ai->map_max_x; x++) {
                    if ((row << x) & 0x8000) {
                        if (selected == 0) {
                            coordinates->fields.x = x;
                            coordinates->fields.y = y;
                            coordinates->fields.elevation = level;
                            coordinates->fields.zero = 0;
                            tiles[level][y] ^= 0x8000 >> x;
                            return 1;
                        }
                        selected--;
                    }
                }
            }
        }
    }
    return 0;
}
