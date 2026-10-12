#include "fft/battle_map.h"
#include "fft/battle_move.h"
#include "psx/types.h"

/* Clearance above a tile for battle_move_clamp_z_to_tile_headroom, in half-height
 * units.
 *
 * When the position's other layer is not blocked and its surface is higher, the
 * result is twice the gap between that layer's underside (its height less its
 * ceiling depth) and a point 3 levels above this tile's surface. Otherwise it is
 * 0x40. */
s32 battle_move_calculate_tile_layer_step_offset(s32 x, s32 y, u32 layer) {
    map_tile_t* from;
    map_tile_t* to;
    s32 from_height;
    s32 to_height;
    s32 limit;

    from = battle_map_get_tile_data_pointer(x, y, layer);
    to = battle_map_get_tile_data_pointer(x, y, layer ^ 1);
    if ((to->flags.blocked) == 0) {
        from_height = from->height + (from->depth_half_height.depth);
        to_height = to->height + (to->depth_half_height.depth);
        if (from_height < to_height) {
            limit = from_height + 3;
            return (to->height - (to->ceiling_depth_and_marks.ceiling_depth) - limit) * 2;
        }
    }
    return 0x40;
}
