#include "fft/battle_gfx.h"
#include "fft/battle_unit.h"

/* Copies the Float display status into BATTLE_MOTION_FLAG_FLOAT and, unless the
 * unit is in a Jump (flag set, being removed, or its height still active), puts
 * the unit back at its tile's height. */
void battle_unit_update_float_and_jump_height(battle_unit_misc_data_t* unit) {
    if (unit->status_flags_5_6 & BATTLE_MISC_STATUS_FLOAT) {
        unit->movement.floating = 1;
    } else {
        unit->movement.floating = 0;
    }
    if ((unit->status_flags_5_6 & (BATTLE_MISC_STATUS_JUMP | BATTLE_MISC_STATUS_JUMP_HEIGHT_ACTIVE)) == 0
        && (unit->statuses_to_remove_5_6 & BATTLE_MISC_STATUS_JUMP) == 0) {
        unit->screen.vy = battle_gfx_calculate_screen_z_from_misc_screen_data(unit);
        battle_unit_set_real_coords_from_screen_coords(unit);
    }
}
