#include "fft/battle.h"
#include "psx/types.h"

/* Convert a tile position (x, layer, y) to world coordinates: the tile centre (28
 * units per tile) at the tile's height, 12 units per level and negative upward.
 * The slope and the water depth are not counted. */
void battle_effect_convert_tile_coords_to_world_coords(s16* in, s32* out) {
    map_tile_t* tile;

    tile = battle_map_get_tile_data_pointer(in[0], in[2], in[1]);
    out[0] = in[0] * 28 + 14;
    out[2] = in[2] * 28 + 14;
    out[1] = -(tile->height * 12);
}
