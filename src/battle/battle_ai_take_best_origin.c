#include "fft/battle_ai.h"

/* Move the best remaining origin tile into the current action's coords and remove it: the lowest
 * tile_priority, then the lowest tile_foe_proximity; ties go to the last tile scanned. The caller
 * guarantees one is left (there is no empty check). */
void battle_ai_take_best_origin(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 best_key = 0x7fffffff;
    s32 x, y, level;
    s32 key;
    u16 row;

    for (level = 0; level < MAP_TERRAIN_LAYER_COUNT; level++) {
        for (y = 0; y < ai->map_max_y; y++) {
            row = ai->origin_tiles[level][y];
            if (row != 0) {
                for (x = 0; x < ai->map_max_x; x++) {
                    if ((row << x) & 0x8000) {
                        s32 priority_scale = 65536;
                        key = ai->tile_priority[level][y][x];
                        key *= priority_scale;
                        key += ai->tile_foe_proximity[level][y][x];
                        if (best_key >= key) {
                            best_key = key;
                            ai->current_action.coords.fields.x = x;
                            ai->current_action.coords.fields.y = y;
                            ai->current_action.coords.fields.elevation = level;
                        }
                    }
                }
            }
        }
    }
    ai->origin_tiles[ai->current_action.coords.fields.elevation][ai->current_action.coords.fields.y]
        ^= 0x8000 >> ai->current_action.coords.fields.x;
    ai->origin_tile_count--;
}
