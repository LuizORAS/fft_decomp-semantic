#include "fft/battle_status.h"
#include "psx/types.h"

/* Disable the renderer flags of every current status, then clear the unit's
 * innate, immune, current, status CT and inflicted status bytes (Morbol
 * transformation preparation). */
void status_reapply_active_flags(battle_stats_t* unit) {
    s32 i;
    s32 battle_id = unit->battle_id;

    for (i = 0; i < STATUS_COUNT; i++) {
        s32 byte_index = i / 8;
        s32 mask = 0x80 >> (i & 7);
        if (unit->status_sets.current[byte_index] & mask) {
            status_queue_runtime_graphics_flag_change(STATUS_HANDLER_INDEX(i), 0, battle_id);
        }
    }
    main_util_clear_byte_data(unit->status_sets.innate, 0x1F);
    main_util_clear_byte_data(unit->inflicted_status, STATUS_BYTE_COUNT);
}
