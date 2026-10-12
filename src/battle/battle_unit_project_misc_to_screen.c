#include "fft/battle_camera.h"
#include "fft/battle_unit.h"

/* Projects a unit's position to display coordinates (x, then y).
 *
 * The missing-unit test compares with -1, but
 * battle_unit_get_screen_data_ptr_by_misc_id returns 0 for a missing unit, so the
 * projection then reads address 0. */
void battle_unit_project_misc_to_screen(u32 misc_id, SVECTOR* screen_coords) {
    SVECTOR* screen_data;

    screen_data = battle_unit_get_screen_data_ptr_by_misc_id(misc_id);
    if (screen_data != (SVECTOR*)-1) {
        battle_camera_project_world_coords_to_screen(screen_data, screen_coords);
    }
}
