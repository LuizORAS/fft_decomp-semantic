#include "fft/battle_unit.h"

/* Unlinks a rider from its mount: both records return to the unmounted state and
 * the rider loses its mounted display flag. Does nothing for a unit that is not a
 * rider. */
void battle_unit_dismount_rider(battle_unit_misc_data_t* unit) {
    battle_unit_misc_data_t* mount;
    if (unit->mount_state == BATTLE_MISC_MOUNT_STATE_RIDER) {
        mount = battle_unit_get_misc_data_by_misc_id(unit->mount_partner_misc_id);
        unit->standing_on_unit_id_plus_one = 0;
        unit->mount_state = BATTLE_MISC_MOUNT_STATE_NONE;
        unit->mount_partner_misc_id = 0;
        unit->status_flags_1_4 = unit->status_flags_1_4 & ~BATTLE_MISC_STATUS_MOUNTED;
        if (mount != 0) {
            mount->mount_state = BATTLE_MISC_MOUNT_STATE_NONE;
            mount->mount_partner_misc_id = 0;
        }
    }
}
