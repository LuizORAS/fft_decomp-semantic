#include "fft/battle_effect.h"
#include "fft/battle_unit.h"

/* Effect-camera twin of battle_unit_copy_world_coords_to_vector (0x801330e4):
 * widens the unit's integer world-coordinate 16-bit values into three 32-bit components
 * (used for effect camera targets). */
void effect_copy_unit_world_coords_to_vector(u32 runtime_id, VECTOR* destination) {
    SVECTOR* source;

    source = battle_unit_get_world_coords_by_runtime_id(runtime_id);
    destination->vx = source->vx;
    destination->vy = source->vy;
    destination->vz = source->vz;
}
