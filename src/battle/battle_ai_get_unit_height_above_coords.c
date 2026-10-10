#include "fft/battle.h"

/* Return how many levels unit's tile stands above the tile at coords: the half-height difference
 * halved, truncated toward 0. Body height and water depth do not count. */
s32 battle_ai_get_unit_height_above_coords(battle_ai_coords_t* coords, battle_stats_t* unit) {
    map_tile_t* tile;
    s32 h1;
    s32 h2;

    tile = &g_battle_map_tile_data[(unit->position.raw >> 15) * 0x100
        + unit->position.bits.y * g_battle_ai_data_base.map_max_x + unit->x];
    h1 = tile->height * 2 + (tile->depth_half_height & MAP_TILE_HALF_HEIGHT_MASK);
    tile = &g_battle_map_tile_data[(coords->bytes.elevation << 8) + coords->bytes.y * g_battle_ai_data_base.map_max_x
        + coords->bytes.x];
    h2 = tile->height * 2 + (tile->depth_half_height & MAP_TILE_HALF_HEIGHT_MASK);
    return (h1 - h2) / 2;
}
