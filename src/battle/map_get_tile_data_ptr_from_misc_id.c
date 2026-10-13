#include "fft/battle_map.h"
#include "psx/types.h"

/* The tile a unit stands on, by misc id. The misc record lookup is not checked:
 * an id without one is read through address 0. */
map_tile_t* map_get_tile_data_ptr_from_misc_id(u32 misc_id) {
    battle_unit_t* unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);

    return map_get_tile_data_pointer(unit->map_x, unit->map_y, unit->map_z);
}
