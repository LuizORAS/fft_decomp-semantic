#include "fft/battle.h"
#include "psx/types.h"

/* Same as battle_unit_get_map_coords_from_misc_id without the result. */
void battle_unit_copy_map_coords_from_misc_id(u32 misc_id, s16* coords) {
    battle_unit_get_map_coords_from_misc_id(misc_id, coords);
}
