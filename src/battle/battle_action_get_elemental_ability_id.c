#include "fft/battle_action.h"
#include "fft/battle_map.h"
#include "psx/types.h"

/* The Elemental ability that the unit's tile selects: g_main_terrain_geomancy_abilities indexed
 * by the tile's surface type. */
s32 battle_action_get_elemental_ability_id(const battle_stats_t* unit) {
    return g_main_terrain_geomancy_abilities[g_battle_map_tile_data[battle_map_calculate_location(unit)].surface.type];
}
