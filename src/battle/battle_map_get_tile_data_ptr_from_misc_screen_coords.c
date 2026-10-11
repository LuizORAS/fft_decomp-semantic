#include "fft/battle.h"

/* The tile under a unit's screen position (28 units per tile) on its map layer. */
map_tile_t* battle_map_get_tile_data_ptr_from_misc_screen_coords(u32 misc_id) {
    battle_unit_misc_data_t* unit;

    unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);
    return battle_map_get_tile_data_pointer(
        unit->screen.vx / MAP_TILE_SIZE, unit->screen.vz / MAP_TILE_SIZE, unit->map_z);
}
