#include "fft/battle_unit.h"

/* The screen position of the unit with this runtime id, or 0 for a missing unit.
 * battle_unit_copy_screen_coords and battle_unit_project_to_screen test
 * the result against -1 instead. */
SVECTOR* battle_unit_get_screen_coords_by_runtime_id(u32 runtime_id) {
    battle_unit_t* unit = battle_unit_get_by_runtime_id(runtime_id & 0xffff);

    if (unit == 0) {
        return 0;
    }
    return &unit->screen;
}
