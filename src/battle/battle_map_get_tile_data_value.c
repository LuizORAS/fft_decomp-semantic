#include "fft/battle_map.h"
#include "psx/types.h"

/* Returns one packed field of the tile at (x, y, layer), or 0xff for an
 * unknown selector. The tile address is not bounds-checked. */
s32 battle_map_get_tile_data_value(s32 field, s32 x, s32 y, s32 layer) {
    map_tile_t* tile;

    tile = &g_battle_map_tile_data[(layer << 8) + y * g_battle_map_max_x + x];
    switch (field) {
    case MAP_TILE_DATA_SURFACE_TYPE:
        return tile->surface.type;
    case MAP_TILE_DATA_UNKNOWN_01:
        return tile->_unknown_01;
    case MAP_TILE_DATA_HEIGHT:
        return tile->height;
    case MAP_TILE_DATA_HALF_HEIGHT:
        return tile->depth_half_height.half_height;
    case MAP_TILE_DATA_DEPTH:
        return tile->depth_half_height.depth;
    case MAP_TILE_DATA_SLOPE_TYPE:
        return tile->slope_type;
    case MAP_TILE_DATA_CEILING_DEPTH:
        return tile->ceiling_depth_and_marks.bits.ceiling_depth;
    case MAP_TILE_DATA_BLOCKED:
        return tile->flags.bits.blocked;
    case MAP_TILE_DATA_UNTARGETABLE:
        return tile->flags.bits.untargetable;
    case MAP_TILE_DATA_SHADOW_MODE:
        return tile->flags.bits.shadow_mode;
    case MAP_TILE_DATA_UNKNOWN_FLAGS_4_6:
        return tile->flags.bits._unknown_00;
    case MAP_TILE_DATA_FLAT_CAMERA_BLOCK_MASK:
        return tile->camera_block_masks & MAP_TILE_CAMERA_BLOCK_DIRECTION_MASK;
    case MAP_TILE_DATA_MOVE_DESTINATION_MARK:
        return tile->ceiling_depth_and_marks.bits.move_destination;
    case MAP_TILE_DATA_ABILITY_RANGE_MARK:
        return tile->ceiling_depth_and_marks.bits.ability_range;
    }
    return 0xFF;
}
