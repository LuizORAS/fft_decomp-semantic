#include "fft/battle.h"
#include "psx/types.h"

/* Apply the removals in unit_idx's action record: clear each status from the inflicted set, end its
 * count (main_status_set_ct) and queue its removal graphics; then rebuild the current set
 * (main_status_store_current). Innate statuses stay. */
void battle_status_apply_pending_removals(s32 unit_idx) {
    battle_stats_t* unit;
    s32 i;
    s32 mask;
    s32 idx;

    unit = &g_battle_unit_stats[unit_idx];
    for (i = 0; i < BATTLE_STATUS_COUNT; i++) {
        idx = i / 8;
        mask = 0x80 >> (i & 7);
        if (unit->action.status_removal[idx] & mask) {
            unit->inflicted_status[idx] = (u8)(unit->inflicted_status[idx] & ~mask);
            if (main_status_set_ct(unit, i, 1) == 0) {
                battle_status_queue_graphics_change_if_executing(BATTLE_STATUS_HANDLER_INDEX(i), 0, unit_idx);
            }
        }
    }
    main_status_store_current(unit);
}
