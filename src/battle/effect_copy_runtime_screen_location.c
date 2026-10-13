#include "fft/battle_effect.h"
#include "fft/battle_unit.h"

/* Effect-camera twin of battle_unit_copy_screen_location (0x801330e4):
 * widens the unit's screen X/Z/Y 16-bit values into three 32-bit components
 * (used for effect camera targets). */
void effect_copy_runtime_screen_location(u32 runtime_id, VECTOR* destination) {
    SVECTOR* screen_data;

    screen_data = battle_unit_get_screen_coords_by_runtime_id(runtime_id);
    destination->vx = screen_data->vx;
    destination->vy = screen_data->vy;
    destination->vz = screen_data->vz;
}
