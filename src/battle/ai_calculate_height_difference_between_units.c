#include "fft/battle_ai.h"
#include "fft/battle_map.h"

/*
 * Compare the unit tile's height with the supplied AI coordinates.
 *
 * Return their signed half-height difference divided by 2, truncating
 * toward 0. unit body height and water depth are not included.
 */
s32 ai_calculate_height_difference_between_units(const ai_coords_t* coords, const battle_stats_t* unit) {
    map_tile_t* tile;
    s32 h1;
    s32 h2;

    tile = &g_map_tile_data[unit->position.higher_elevation * MAP_TILES_PER_LAYER
        + unit->position.y * g_ai_data_base.map_max_x + unit->x];
    h1 = tile->height * 2 + tile->depth_half_height.half_height;
    tile = &g_map_tile_data[(coords->fields.elevation * MAP_TILES_PER_LAYER)
        + coords->fields.y * g_ai_data_base.map_max_x + coords->fields.x];
    h2 = tile->height * 2 + tile->depth_half_height.half_height;
    return (h1 - h2) / 2;
}
