#include "fft/battle_unit.h"
#include "fft/world.h"
#include "psx/types.h"

s32 world_unit_get_map_coords_by_runtime_id(s32 runtime_id, SVECTOR* out) {
    return battle_unit_get_map_coords_by_runtime_id(runtime_id, out);
}
