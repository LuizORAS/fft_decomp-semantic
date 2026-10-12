#include "fft/battle.h"
#include "fft/world.h"

/* Copy a Misc Unit's renderer-space coordinates into the world's coordinate
 * triple. A -1 lookup result leaves the destination unchanged.
 */
vec3u16_t* world_copy_misc_unit_screen_coords(u32 misc_id) {
    SVECTOR* source;
    vec3u16_t* dest;

    source = battle_unit_get_screen_data_ptr_by_misc_id(misc_id);

    if (source != (SVECTOR*)-1) {
        dest = &g_world_current_misc_screen_coords;
        dest->x = source->vx;
        dest->y = source->vy;
        dest->z = source->vz;
        return dest;
    }
    return (vec3u16_t*)-1;
}
