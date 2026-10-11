#include "fft/main.h"
#include "psx/types.h"

/* Pick the status change of an action result to show: among its inflictions and removals, the one
 * whose status has the highest order in the status data. Returns the status index + 1, plus 0x80 for
 * a removal and 0x100 when the status has STATUS_EFFECT_FLAG_2_SCRIPT_FILTER_EXCLUDED (the preview never acts
 * on it, QUIRKS.md); 0 when there is none. The preview display calls it on the main stack. */
s32 main_status_find_action_highest_order_effect(const battle_action_data_t* action) {
    s32 highest_order;
    s32 result;
    s32 status_index;
    u8 mask;
    s32 byte_index;
    u8 current_order;

    highest_order = -1;
    result = 0;
    for (status_index = 0; status_index < BATTLE_STATUS_COUNT; status_index++) {
        byte_index = status_index / 8;
        mask = 0x80 >> (status_index & 7);
        if (action->status_removal[byte_index] & mask) {
            current_order = g_main_status_effect_data[status_index].order;
            if (current_order > highest_order) {
                highest_order = current_order;
                result = status_index + 0x81;
                if (g_main_status_effect_data[status_index].flags_2 & STATUS_EFFECT_FLAG_2_SCRIPT_FILTER_EXCLUDED) {
                    result = status_index + 0x181;
                }
            }
        }
        if (action->status_infliction[byte_index] & mask) {
            current_order = g_main_status_effect_data[status_index].order;
            if (current_order > highest_order) {
                highest_order = current_order;
                result = status_index + 1;
                if (g_main_status_effect_data[status_index].flags_2 & STATUS_EFFECT_FLAG_2_SCRIPT_FILTER_EXCLUDED) {
                    result = status_index + 0x101;
                }
            }
        }
    }
    return result;
}
