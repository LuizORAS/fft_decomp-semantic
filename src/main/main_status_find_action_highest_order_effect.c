#include "fft/main.h"
#include "psx/types.h"

/* Pick the status change of an action result to show: among its inflictions and removals, the one
 * whose status has the highest order in the status data. Returns the status index + 1, plus 0x80 for
 * a removal and 0x100 when the status's flags_2 bit 0x08 is set; 0 when there is none. The preview
 * display calls it on the main stack. */
s32 main_status_find_action_highest_order_effect(const battle_action_data_t* action) {
    s32 highest_order;
    s32 result;
    s32 status_index;
    s32 mask;
    s32 status_mask;
    s32 status_change;
    s32 byte_index;
    u8 current_order;

    highest_order = -1;
    result = 0;
    for (status_index = 0; status_index < BATTLE_STATUS_COUNT; status_index++) {
        byte_index = status_index / 8;
        mask = 0x80 >> (status_index & 7);
        /* Keeps the shift ahead of the status-byte load instead of in its delay slot. */
        __asm__("" : : "r"(mask));
        status_change = action->status_removal[byte_index] & mask;
        status_mask = mask;
        if (status_change != 0) {
            current_order = g_main_status_effect_data[status_index].order;
            if (current_order > highest_order) {
                highest_order = current_order;
                result = status_index + 0x81;
                if (g_main_status_effect_data[status_index].flags_2 & STATUS_EFFECT_FLAG_2_PROVISIONAL_CHECK_SET_8) {
                    result = status_index + 0x181;
                }
            }
        }
        if (status_mask & action->status_infliction[byte_index]) {
            current_order = g_main_status_effect_data[status_index].order;
            if (current_order > highest_order) {
                highest_order = current_order;
                result = status_index + 1;
                if (g_main_status_effect_data[status_index].flags_2 & STATUS_EFFECT_FLAG_2_PROVISIONAL_CHECK_SET_8) {
                    result = status_index + 0x101;
                }
            }
        }
    }
    return result;
}
