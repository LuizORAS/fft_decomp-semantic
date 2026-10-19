#include "fft/battle_ai.h"
#include "fft/battle_map.h"

/* Return 1 when the tile at coords gives the considered Elemental ability: the entry of its surface
 * type in g_main_terrain_geomancy_abilities. */
s32 battle_ai_can_elemental_ability_be_used(const battle_ai_coords_t* coords) {
    u32 tile_id;
    u32 tile_type;
    u32 ability_terrain;

    tile_id = (coords->fields.elevation * MAP_TILES_PER_LAYER) + coords->fields.y * g_battle_ai_data_base.map_max_x
        + coords->fields.x;
    tile_type = g_battle_map_tile_data[tile_id].surface.type;
    ability_terrain = g_main_terrain_geomancy_abilities[tile_type];
    return ability_terrain == g_battle_ai_data_base.considered_ability.ability_id;
}
