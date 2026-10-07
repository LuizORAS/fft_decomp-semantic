#include "fft/battle.h"
#include "psx/types.h"

/* Clamp a flying unit's screen height (value; negative is up) over the tile at (x, y) on layer:
 * no lower than 4 levels (0x30) above the tile's surface, and no higher than the room under a
 * tile above it (battle_move_calculate_tile_layer_step_offset); when that room is lower than 4
 * levels, the ceiling wins. The fly steps use it to find the run of tiles they cross at one
 * height. */
s32 battle_move_clamp_z_to_tile_headroom(s32 value, s32 x, s32 y, u32 layer) {
    map_tile_t* tile;
    s32 clearance;
    s32 height;
    s32 lo;
    s32 hi;
    u32 sub;

    tile = battle_map_get_tile_data_pointer(x, y, layer);
    clearance = battle_move_calculate_tile_layer_step_offset(x, y, layer);
    sub = tile->depth_half_height;
    height = ((tile->height + (sub >> MAP_TILE_DEPTH_SHIFT)) * 2) + (sub & MAP_TILE_HALF_HEIGHT_MASK);
    lo = -(height + clearance) * 6;
    if (value < lo) {
        return lo;
    }
    hi = (-height * 6) - 0x30;
    if (value >= hi) {
        if (hi < lo) {
            return lo;
        }
        return hi;
    }
    return value;
}
