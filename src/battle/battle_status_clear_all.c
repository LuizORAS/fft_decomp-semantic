#include "fft/battle_status.h"
#include "fft/memory.h"
#include "psx/types.h"

/* Remove every status for a Morbol transformation: queue the removal graphics of each current
 * status, then clear the unit's innate, immunity and current sets, status CTs and inflicted statuses
 * (battle_action_apply_target_result, MORBOL effect, while executing). */
void battle_status_clear_all(battle_stats_t* unit) {
    s32 i;
    s32 battle_id = unit->battle_id;

    for (i = 0; i < BATTLE_STATUS_COUNT; i++) {
        s32 byte_index = i / 8;
        s32 mask = 0x80 >> (i & 7);
        if (unit->status_sets.current[byte_index] & mask) {
            battle_status_queue_runtime_graphics_flag_change(BATTLE_STATUS_HANDLER_INDEX(i), 0, battle_id);
        }
    }
    main_mem_zero(unit->status_sets.innate, 0x1F);
    main_mem_zero(unit->inflicted_status, BATTLE_STATUS_BYTE_COUNT);
}
