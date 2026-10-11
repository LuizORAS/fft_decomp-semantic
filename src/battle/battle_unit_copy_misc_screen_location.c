#include "fft/battle.h"

/* Widens a unit's screen position halfwords into a word triple. A missing unit is
 * not tested for: the copy then reads address 0. */
void battle_unit_copy_misc_screen_location(u32 misc_id, VECTOR* destination) {
    SVECTOR* screen_data;

    screen_data = battle_unit_get_screen_data_ptr_by_misc_id(misc_id);
    destination->vx = screen_data->vx;
    destination->vy = screen_data->vy;
    destination->vz = screen_data->vz;
}
