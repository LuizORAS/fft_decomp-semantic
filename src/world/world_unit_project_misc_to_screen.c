#include "fft/battle_camera.h"
#include "fft/battle_unit.h"
#include "fft/world.h"

/* Project a displayed unit's position to screen coordinates; nothing is
 * written when the unit has no screen data. */
void world_unit_project_misc_to_screen(s32 misc_id, SVECTOR* screen_coords) {
    SVECTOR* world_coords = battle_unit_get_screen_data_ptr_by_misc_id(misc_id);

    if (world_coords != (SVECTOR*)-1) {
        battle_camera_project_world_coords_to_screen(world_coords, screen_coords);
    }
}
