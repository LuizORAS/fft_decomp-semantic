#include "fft/battle.h"
#include "psx/types.h"

/* Return 1 when the AI cannot target unit_id: absent, Crystal or Treasure, a ridden mount, or a
 * Transparent enemy, except a rider whose mount is Transparent too (QUIRKS.md). Any other unit that is
 * not an enemy is targetable.
 *
 * The mount is read from the low 4 bits of mount_info, where other mount code uses 5
 * (BATTLE_MOUNT_INFO_PARTNER_ID_MASK). */
s32 battle_ai_is_unit_untargetable(s32 unit_idx) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    battle_stats_t* unit;
    s32 mount;

    unit = &g_battle_unit_stats[unit_idx];
    if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE) {
        return 1;
    }
    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRYSTAL)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRYSTAL))
        || (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TREASURE)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TREASURE))) {
        return 1;
    }
    mount = unit->mount_info;
    if (mount & BATTLE_MOUNT_INFO_FLAG_MOUNT) {
        return 1;
    }
    if (ai->unit_decisions[unit_idx].enemy_flag == 0) {
        return 0;
    }
    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TRANSPARENT)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TRANSPARENT))
        && (!(mount & BATTLE_MOUNT_INFO_FLAG_RIDER)
            || !(g_battle_unit_stats[mount & 0xf]
                     .status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TRANSPARENT)]
                & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TRANSPARENT)))) {
        return 1;
    }
    return 0;
}
