#include "fft/battle_ai.h"
#include "fft/battle_map.h"

/* Return how many levels unit's tile stands above the tile at coords: the half-height difference
 * halved, truncated toward 0. Body height and water depth do not count. */
s32 battle_ai_get_unit_height_above_coords(const battle_ai_coords_t* coords, const battle_stats_t* unit) {
    map_tile_t* tile;
    s32 h1;
    s32 h2;

    tile = &g_battle_map_tile_data[unit->position.higher_elevation * MAP_TILES_PER_LAYER
        + unit->position.y * g_battle_ai_data_base.map_max_x + unit->x];
    h1 = tile->height * 2 + tile->depth_half_height.half_height;
    tile = &g_battle_map_tile_data[(coords->fields.elevation * MAP_TILES_PER_LAYER)
        + coords->fields.y * g_battle_ai_data_base.map_max_x + coords->fields.x];
    h2 = tile->height * 2 + tile->depth_half_height.half_height;
    return (h1 - h2) / 2;
}
