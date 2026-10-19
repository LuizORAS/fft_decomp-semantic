#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* WORLD's version of battle_menu_get_cursor_bob_offset: the same lookup of the thread's frame
 * counter (32-bit task value 0) in the idle or active threshold table. */
s32 world_menu_get_cursor_bob_offset(s32 mode) {
    s32 value = g_thread_contexts[g_thread_current_id].task_words[0];
    battle_menu_cursor_bob_entry_t* table;
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
