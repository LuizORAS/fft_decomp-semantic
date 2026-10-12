#include "fft/battle_unit.h"
#include "fft/world.h"

/* Copy a Misc Unit's renderer-space coordinates into the world's coordinate
 * triple. A -1 lookup result leaves the destination unchanged.
 */
vec3u16_t* world_copy_misc_unit_screen_coords(u32 misc_id) {
    SVECTOR* source;

    source = battle_unit_get_screen_data_ptr_by_misc_id(misc_id);

    if (source != (SVECTOR*)-1) {
        g_world_misc_unit_screen_coords.x = source->vx;
        g_world_misc_unit_screen_coords.y = source->vy;
        g_world_misc_unit_screen_coords.z = source->vz;
        return &g_world_misc_unit_screen_coords;
    }
    return (vec3u16_t*)-1;
}
