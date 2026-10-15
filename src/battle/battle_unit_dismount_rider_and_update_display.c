#include "fft/battle.h"
#include "psx/types.h"

/* Same as battle_unit_dismount_rider, followed by a display update of the rider and of the mount. */
void battle_unit_dismount_rider_and_update_display(battle_unit_misc_data_t* rider) {
    battle_unit_misc_data_t* mount;

    if (rider->mount_state == BATTLE_MISC_MOUNT_STATE_RIDER) {
        mount = battle_unit_get_misc_data_by_misc_id(rider->mount_partner_misc_id);
        rider->standing_on_unit_id_plus_one = 0;
        rider->mount_state = BATTLE_MISC_MOUNT_STATE_NONE;
        rider->mount_partner_misc_id = 0;
        rider->status_flags_1_4 &= ~BATTLE_MISC_STATUS_MOUNTED;
        battle_unit_update_display_by_misc_id(rider->unit_id);
        if (mount != 0) {
            mount->mount_state = BATTLE_MISC_MOUNT_STATE_NONE;
            mount->mount_partner_misc_id = 0;
            battle_unit_update_display_by_misc_id(mount->unit_id);
        }
    }
}
