#include "fft/battle_unit.h"

/* Widens a unit's world position 16-bit values (world_coords) into a triple of 32-bit values. A missing unit is
 * not tested for: the copy then reads address 0. BATTLE compiles this text twice; the effect camera's
 * copy, effect_copy_unit_world_coords_to_vector, takes its targets this way. */
void unit_copy_world_coords_to_vector(u32 runtime_id, VECTOR* destination) {
    SVECTOR* source;

    source = battle_unit_get_world_coords_by_runtime_id(runtime_id);
    destination->vx = source->vx;
    destination->vy = source->vy;
    destination->vz = source->vz;
}
