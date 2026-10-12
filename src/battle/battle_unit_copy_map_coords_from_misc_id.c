#include "fft/battle_unit.h"
#include "psx/types.h"

void battle_unit_copy_map_coords_from_misc_id(u32 misc_id, SVECTOR* coords) {
    battle_unit_get_map_coords_from_misc_id(misc_id, coords);
}
