#include "fft/battle_ai.h"
#include "fft/battle_map.h"

/*
 * Test whether the terrain selects the considered Geomancy ability.
 *
 * Return 1 for the terrain table's matching ability ID, otherwise 0.
 */
s32 ai_can_use_geomancy(const ai_coords_t* coords) {
    u32 tile_id;
    u32 tile_type;
    u32 ability_terrain;

    tile_id = (coords->bytes.elevation * MAP_TILES_PER_LAYER) + coords->bytes.y * g_ai_data_base.map_max_x
        + coords->bytes.x;
    tile_type = g_map_tile_data[tile_id].surface.type;
    ability_terrain = g_main_terrain_geomancy_abilities[tile_type];
    return ability_terrain == g_ai_data_base.considered_ability.ability_id;
}
