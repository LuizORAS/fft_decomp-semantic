#include "fft/battle_unit.h"

/* Copy a runtime Unit's renderer-space coordinates into the shared coordinate
 * triple. A -1 lookup result leaves the destination unchanged.
 */
SVECTOR* battle_unit_copy_screen_coords_to_scratch(u32 runtime_id) {
    SVECTOR* source;
    SVECTOR* dest;

    source = battle_unit_get_screen_coords_by_runtime_id(runtime_id);

    if (source != (SVECTOR*)-1) {
        dest = &g_battle_unit_runtime_coords;
        dest->vx = source->vx;
        g_battle_unit_runtime_coords.vy = source->vy;
        g_battle_unit_runtime_coords.vz = source->vz;
        return dest;
    }
    return (SVECTOR*)-1;
}
