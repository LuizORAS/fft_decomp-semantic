#include "fft/battle_map.h"

/* The tile under a unit's screen position (28 units per tile) on its map layer. */
map_tile_t* map_get_unit_tile_from_runtime_screen_coords(u32 runtime_id) {
    battle_unit_t* unit;

    unit = battle_unit_get_by_runtime_id(runtime_id & 0xffff);
    return map_get_tile_data_pointer(unit->screen.vx / MAP_TILE_SIZE, unit->screen.vz / MAP_TILE_SIZE, unit->map_z);
}
