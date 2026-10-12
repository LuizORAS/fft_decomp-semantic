#include "fft/battle.h"

/* Sets BATTLE_MOTION_FLAG_SUPPRESS_FACING_UPDATE on a unit and clears its 16-bit sprite
 * display flags. */
void battle_unit_set_facing_update_suppression(u32 misc_id) {
    battle_unit_misc_data_t* unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);

    if (unit != 0) {
        unit->sprite_display_flags = 0;
        unit->movement.suppress_facing_update = 1;
    }
}
