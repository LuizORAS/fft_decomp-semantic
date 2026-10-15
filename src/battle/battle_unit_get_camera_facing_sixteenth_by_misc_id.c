#include "fft/battle.h"

/* The unit's camera_facing_sixteenth modulo 16, or -1 when no record has the misc
 * id. No retail code calls it. */
s16 battle_unit_get_camera_facing_sixteenth_by_misc_id(u32 misc_id) {
    battle_unit_misc_data_t* unit;

    unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);
    if (unit != 0) {
        return unit->camera_facing_sixteenth.s % 0x10;
    }
    return -1;
}
