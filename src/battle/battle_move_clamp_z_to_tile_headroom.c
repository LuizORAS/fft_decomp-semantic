#include "fft/battle_map.h"
#include "fft/battle_move.h"
#include "psx/types.h"

/*
 * Clamps a camera/height value into the walkable band above one tile: the
 * lower bound subtracts the layer step clearance from battle_move_calculate_tile_layer_step_offset, the
 * upper bound is the tile surface height alone less 0x30.
 */
s32 battle_move_clamp_z_to_tile_headroom(s32 value, s32 x, s32 y, u32 layer) {
    map_tile_t* tile;
    s32 clearance;
    s32 height;
    s32 lo;
    s32 hi;
    map_tile_depth_t depth;

    tile = battle_map_get_tile_data_pointer(x, y, layer);
    clearance = battle_move_calculate_tile_layer_step_offset(x, y, layer);
    depth = tile->depth_half_height;
    height = ((tile->height + depth.depth) * 2) + depth.half_height;
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
