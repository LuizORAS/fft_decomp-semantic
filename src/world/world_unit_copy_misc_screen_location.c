#include "fft/battle_unit.h"
#include "fft/world.h"

/* Widen a displayed unit's 16-bit screen x, y and z values into a 32-bit coordinate triple. */
void world_unit_copy_misc_screen_location(u32 misc_id, VECTOR* destination) {
    SVECTOR* screen_data;

    screen_data = battle_unit_get_screen_data_ptr_by_misc_id(misc_id);
    destination->vx = screen_data->vx;
    destination->vy = screen_data->vy;
    destination->vz = screen_data->vz;
}
