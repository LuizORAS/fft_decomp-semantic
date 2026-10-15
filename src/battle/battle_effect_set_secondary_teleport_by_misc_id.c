#include "fft/battle.h"

/* Show the teleport effect on the unit whose misc id is the low halfword of misc_id and return 1, or 0
 * when there is none. The event instructions that teleport a unit out and in call it. */
s32 battle_effect_set_secondary_teleport_by_misc_id(u32 misc_id) {
    battle_unit_misc_data_t* unit;

    unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);
    if (unit != 0) {
        battle_effect_set_secondary_teleport(unit);
        return 1;
    }
    return 0;
}
