#include "fft/battle.h"
#include "psx/types.h"

enum {
    TILE_CEILING_NONE = 0x7f,
    TILE_ELEVATION_STRIDE = 0x100,
};

/*
 * Returns the lowest ceiling above (x, y) across the two elevation layers, or
 * 0x7f when no tile above exit_height forms one.  exit_slope_shift selects the
 * exit side's two slope bits; exit_height is the exit side height (or tile
 * height plus unit size).
 */
u8 battle_move_calculate_tile_ceiling(s32 x, s32 y, s32 exit_slope_shift, u8 exit_height) {
    s32 elevation;
    map_tile_t* tile;
    u8 ceiling;
    u8 tile_ceiling;

    ceiling = TILE_CEILING_NONE;
    for (elevation = 0; elevation < 2; elevation++) {
        tile = &g_battle_map_tile_data[elevation * TILE_ELEVATION_STRIDE + y * g_battle_move_config_ptr->map_max_x + x];
        if ((tile->flags_06.value & MAP_TILE_FLAG_BLOCKED) == 0) {
            tile_ceiling = (tile->height << 1)
                + (tile->depth_half_height & MAP_TILE_HALF_HEIGHT_MASK) * ((tile->slope_type >> exit_slope_shift) & 3)
                - ((tile->ceiling_depth_and_marks & MAP_TILE_CEILING_DEPTH_MASK) << 1);
            if (tile_ceiling > exit_height) {
                if (ceiling < tile_ceiling) {
                    tile_ceiling = ceiling;
                }
                ceiling = tile_ceiling;
            }
        }
    }
    return ceiling;
}
