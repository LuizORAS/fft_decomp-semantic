#include "fft/battle.h"
#include "fft/world.h"

/* Copy a Misc Unit's renderer-space coordinates into the world's coordinate
 * triple. A -1 lookup result leaves the destination unchanged.
 */
vec3u16_t* world_copy_misc_unit_screen_coords(u32 misc_id) {
    battle_screen_coords_t* source;
    vec3u16_t* dest;

    source = battle_unit_get_screen_data_ptr_by_misc_id(misc_id);

    if (source != (battle_screen_coords_t*)-1) {
        dest = &g_world_misc_unit_screen_coords;
        dest->x = source->x;
        dest->y = source->z;
        dest->z = source->y;
        return dest;
    }
    return (vec3u16_t*)-1;
}
