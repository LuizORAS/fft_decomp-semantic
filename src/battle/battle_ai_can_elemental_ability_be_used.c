#include "fft/battle.h"

/* Return 1 when the tile at coords gives the considered Elemental ability: the entry of its surface
 * type in g_main_terrain_geomancy_abilities. */
s32 battle_ai_can_elemental_ability_be_used(battle_ai_coords_t* coords) {
    u32 tile_id;
    u32 tile_type;
    u32 ability_terrain;

    tile_id = (coords->bytes.elevation << 8) + coords->bytes.y * g_battle_ai_data_base.map_max_x + coords->bytes.x;
    tile_type = g_battle_map_tile_data[tile_id].surface.value & MAP_SURFACE_MASK;
    ability_terrain = g_main_terrain_geomancy_abilities[tile_type];
    return (ability_terrain ^ g_battle_ai_data_base.considered_ability.ability_id) < 1;
}
