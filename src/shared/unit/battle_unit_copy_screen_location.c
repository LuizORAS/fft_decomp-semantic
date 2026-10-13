#include "fft/battle_unit.h"

/* Widens a unit's screen position 16-bit values into three 32-bit components. A missing unit is
 * not tested for: the copy then reads address 0. */
void battle_unit_copy_screen_location(u32 runtime_id, VECTOR* destination) {
    SVECTOR* screen_data;

    screen_data = battle_unit_get_screen_coords_by_runtime_id(runtime_id);
    destination->vx = screen_data->vx;
    destination->vy = screen_data->vy;
    destination->vz = screen_data->vz;
}
