#include "fft/battle_map.h"
#include "fft/battle_target.h"

/* Store a unit's tile and report whether targeting must reject the unit. */
s32 target_is_unit_untargetable_and_store_tile(s32 unit_id, s32* tile_index) {
    battle_stats_t* unit = &g_battle_unit_stats[unit_id];

    *tile_index = map_calculate_location(unit);
    if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE) {
        return 1;
    } else if (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_CRYSTAL)]
        & (STATUS_BYTE_MASK(STATUS_ID_CRYSTAL) | STATUS_BYTE_MASK(STATUS_ID_JUMP))) {
        return 1;
    } else if (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_TREASURE)]
        & STATUS_BYTE_MASK(STATUS_ID_TREASURE)) {
        return 1;
    } else if (unit->mount_info & BATTLE_MOUNT_INFO_FLAG_MOUNT) {
        return 1;
    }
    return 0;
}
