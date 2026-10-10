#include "fft/battle.h"

/* Copies a unit's screen position as three halfwords.
 *
 * The missing-unit test compares with -1, but
 * battle_unit_get_screen_data_ptr_by_misc_id returns 0 for a missing unit, so the
 * copy then reads address 0. */
void battle_unit_copy_misc_screen_coords(u32 misc_id, s16* screen_coords) {
    SVECTOR* screen_data;

    screen_data = battle_unit_get_screen_data_ptr_by_misc_id(misc_id);
    if (screen_data != (SVECTOR*)-1) {
        screen_coords[0] = screen_data->vx;
        screen_coords[1] = screen_data->vy;
        screen_coords[2] = screen_data->vz;
    }
}
