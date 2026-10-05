#include "fft/battle.h"
#include "psx/types.h"

/* Return the headroom, in half-heights, between a point 3 levels above the surface of the tile
 * at (x, y) on layer and the underside (height minus ceiling depth) of a higher tile on the other
 * layer, or 0x40 when the other layer holds no higher unblocked tile.
 * battle_move_clamp_z_to_tile_headroom uses it as the flying ceiling. */
s32 battle_move_calculate_tile_layer_step_offset(s32 x, s32 y, u32 layer) {
    map_tile_t* from;
    map_tile_t* to;
    s32 from_height;
    s32 to_height;
    s32 limit;

    from = battle_map_get_tile_data_pointer(x, y, layer);
    to = battle_map_get_tile_data_pointer(x, y, layer ^ 1);
    if ((to->flags_06.value & MAP_TILE_FLAG_BLOCKED) == 0) {
        from_height = from->height + (from->depth_half_height >> MAP_TILE_DEPTH_SHIFT);
        to_height = to->height + (to->depth_half_height >> MAP_TILE_DEPTH_SHIFT);
        if (from_height < to_height) {
            limit = from_height + 3;
            return (to->height - (to->ceiling_depth_and_marks & MAP_TILE_CEILING_DEPTH_MASK) - limit) * 2;
        }
    }
    return 0x40;
}
