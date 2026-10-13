#include "fft/battle_unit.h"

/* Copies a unit's screen position as three 16-bit values.
 *
 * The missing-unit test compares with -1, but
 * battle_unit_get_screen_coords_by_runtime_id returns 0 for a missing unit, so the
 * copy then reads address 0. */
void battle_unit_copy_screen_coords(u32 runtime_id, SVECTOR* screen_coords) {
    SVECTOR* screen_data;

    screen_data = battle_unit_get_screen_coords_by_runtime_id(runtime_id);
    if (screen_data != (SVECTOR*)-1) {
        screen_coords->vx = screen_data->vx;
        screen_coords->vy = screen_data->vy;
        screen_coords->vz = screen_data->vz;
    }
}
