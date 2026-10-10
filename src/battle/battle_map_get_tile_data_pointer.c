#include "fft/battle.h"
#include "psx/types.h"

/* The tile at (x, y) of a layer, or 0 outside the map or for a layer other than 0
 * and 1. */
map_tile_t* battle_map_get_tile_data_pointer(s32 x, s32 y, u32 layer) {
    s32 max_x = g_battle_map_max_x;
    s32 max_y = g_battle_map_max_y;
    if (x < 0 || x >= max_x || y < 0 || y >= max_y || layer >= 2U) {
        return 0;
    }
    return &g_battle_map_tile_data[(layer << 8) + y * max_x + x];
}
