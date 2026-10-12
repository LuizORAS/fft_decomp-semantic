#include "fft/battle_unit.h"

/* The screen position of the unit in slot battle_id, or 0. No retail code calls
 * it. */
void* battle_unit_get_screen_data_ptr_by_battle_id(u32 battle_id) {
    battle_unit_misc_data_t* unit = battle_unit_get_misc_data_by_battle_id(battle_id & 0xffff);

    if (unit == 0) {
        return 0;
    }
    return &unit->screen.vx;
}
