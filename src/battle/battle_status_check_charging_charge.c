#include "fft/battle.h"
#include "psx/types.h"

/* Return 1 when the unit, or the rider a mount carries, is charging a Charge command (Charging, a
 * charged CT, last skillset Charge); with cancel set it also clears that unit's action state
 * (battle_status_clear_action_state_2). A knockback (battle_action_apply_target_result) and a
 * mount's move (the post-move events) cancel the Charge this way. */
s32 battle_status_check_charging_charge(battle_stats_t* unit, s32 cancel) {
    u8 mount_info;

    for (;;) {
        if ((unit->status_sets.current[0] & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARGING))
            && unit->charged_ability_ct != 0xFF && unit->last_skillset_id == SKILLSET_ID_CHARGE) {
            if (cancel != 0) {
                battle_status_clear_action_state_2(unit);
            }
            return 1;
        }
        mount_info = unit->mount_info;
        if (!(mount_info & BATTLE_MOUNT_INFO_FLAG_MOUNT)) {
            break;
        }
        unit = &g_battle_unit_stats[mount_info & BATTLE_MOUNT_INFO_PARTNER_ID_MASK];
    }
    return 0;
}
