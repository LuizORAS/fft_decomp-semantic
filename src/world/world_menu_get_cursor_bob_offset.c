#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Map the current thread's first parameter through a threshold table:
 * mode 0 reduces it modulo the table's first byte first, mode 1 uses the
 * second table as-is. Returns the value paired with the first threshold
 * above the parameter, or 0. */
s32 world_menu_get_cursor_bob_offset(s32 mode) {
    s32 value = g_native_thread_contexts[g_native_current_thread_id].task_words[0];
    menu_cursor_bob_entry_t* table;
    s32 threshold;
    s32 result;

    /* Any other mode leaves `table` unset. */
    if (mode == 0) {
        table = g_world_cursor_bob_thresholds;
        value = value % table[0].threshold;
    } else if (mode == 1) {
        table = g_world_cursor_bob_active_thresholds;
    }
    for (;;) {
        table++;
        threshold = table[0].threshold;
        if (threshold == 0) {
            result = 0;
            break;
        }
        if (value < threshold) {
            result = table[0].offset;
            break;
        }
    }
    return result;
}
