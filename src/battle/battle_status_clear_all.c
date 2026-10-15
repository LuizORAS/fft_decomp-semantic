#include "fft/battle.h"
#include "psx/types.h"

/* Remove every status for a Morbol transformation: queue the removal graphics of each current
 * status, then clear the unit's innate, immunity and current sets, status CTs and inflicted statuses
 * (battle_action_apply_target_result, MORBOL effect, while executing). */
void battle_status_clear_all(battle_stats_t* unit) {
    s32 i;
    s32 misc_unit_id = unit->misc_unit_id;

    i = 0;
    do {
        s32 byte_index = i / 8;
        s32 mask = 0x80 >> (i & 7);
        if (unit->status_sets.current[byte_index] & mask) {
            battle_status_queue_misc_graphics_flag_change(BATTLE_STATUS_HANDLER_INDEX(i), 0, misc_unit_id);
        }
        i += 1;
    } while (i < BATTLE_STATUS_COUNT);
    main_util_clear_byte_data(unit->status_sets.innate, 0x1F);
    main_util_clear_byte_data(unit->inflicted_status, BATTLE_STATUS_BYTE_COUNT);
}
