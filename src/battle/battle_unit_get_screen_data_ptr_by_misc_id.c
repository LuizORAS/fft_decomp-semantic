#include "fft/battle_unit.h"

/* The screen position of the unit with this misc id, or 0 for a missing unit.
 * battle_unit_copy_misc_screen_coords and battle_unit_project_misc_to_screen test
 * the result against -1 instead. */
SVECTOR* battle_unit_get_screen_data_ptr_by_misc_id(u32 misc_id) {
    battle_unit_misc_data_t* unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);

    if (unit == 0) {
        return 0;
    }
    return &unit->screen;
}
