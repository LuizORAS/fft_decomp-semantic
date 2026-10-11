#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

/* WORLD's call of battle_unit_get_map_coords_from_misc_id: a displayed unit's map coordinates into
 * out. */
s32 world_unit_get_map_coords_from_misc_id(s32 misc_id, s16* out) {
    return battle_unit_get_map_coords_from_misc_id(misc_id, out);
}
