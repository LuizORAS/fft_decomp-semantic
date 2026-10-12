#include "fft/battle_action.h"
#include "fft/battle_unit.h"

/* Copy a Misc Unit's renderer-space coordinates into the shared coordinate
 * triple. A -1 lookup result leaves the destination unchanged.
 */
SVECTOR* battle_copy_misc_unit_screen_coords(u32 misc_id) {
    SVECTOR* source;
    SVECTOR* dest;

    source = battle_unit_get_screen_data_ptr_by_misc_id(misc_id);

    if (source != (SVECTOR*)-1) {
        dest = &g_battle_current_misc_screen_coords;
        dest->vx = source->vx;
        g_battle_current_misc_screen_coords.vy = source->vy;
        g_battle_current_misc_screen_coords.vz = source->vz;
        return dest;
    }
    return (SVECTOR*)-1;
}
