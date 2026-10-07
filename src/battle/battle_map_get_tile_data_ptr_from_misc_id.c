#include "fft/battle.h"
#include "psx/types.h"

/* The tile of the map square of the unit with that misc id (battle_map_get_tile_data_pointer). The
 * unit lookup is not checked (QUIRKS.md). */
map_tile_t* battle_map_get_tile_data_ptr_from_misc_id(u32 misc_id) {
    battle_unit_misc_data_t* unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);

    return battle_map_get_tile_data_pointer(unit->map_x, unit->map_y, unit->map_z);
}
