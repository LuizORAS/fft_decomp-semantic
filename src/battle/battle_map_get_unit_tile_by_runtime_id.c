#include "fft/battle_map.h"
#include "psx/types.h"

/* The tile of the map square of the unit with that runtime id (battle_map_get_tile_data_pointer).
 * The unit lookup is not checked: an id without a runtime record is read through address 0
 * (QUIRKS.md). */
map_tile_t* battle_map_get_unit_tile_by_runtime_id(u32 runtime_id) {
    battle_unit_t* unit = battle_unit_get_by_runtime_id(runtime_id & 0xffff);

    return battle_map_get_tile_data_pointer(unit->map_x, unit->map_y, unit->map_z);
}
