#include "fft/battle_map.h"
#include "fft/battle_target.h"

/* Store the unit's tile index and report whether it cannot be a target: an empty slot, a crystal, a
 * jumping unit, a treasure chest, or a mount carrying a rider. */
s32 battle_target_is_unit_untargetable_and_store_tile(s32 unit_id, s32* tile_index) {
    battle_stats_t* unit = &g_battle_unit_stats[unit_id];

    *tile_index = battle_map_calculate_location(unit);
    if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE) {
        return 1;
    } else if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRYSTAL)]
        & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRYSTAL) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_JUMP))) {
        return 1;
    } else if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TREASURE)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TREASURE)) {
        return 1;
    } else if (unit->mount_info & BATTLE_MOUNT_INFO_FLAG_MOUNT) {
        return 1;
    }
    return 0;
}
