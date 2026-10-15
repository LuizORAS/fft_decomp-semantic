#include "fft/battle.h"

/* The unit's facing in quarter turns (0..3), or -1 when no record has the misc
 * id. */
s16 battle_unit_get_facing_quadrant_by_misc_id(u32 misc_id) {
    battle_unit_misc_data_t* unit;

    unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);
    if (unit != 0) {
        return (unit->facing / BATTLE_FACING_QUARTER_TURN) % 4;
    }
    return -1;
}
