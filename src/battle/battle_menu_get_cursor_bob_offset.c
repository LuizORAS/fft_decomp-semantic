#include "fft/battle.h"

typedef struct battle_menu_cursor_bob_threshold {
    s8 threshold;
    s8 offset;
} battle_menu_cursor_bob_threshold_t;

extern battle_menu_cursor_bob_threshold_t g_battle_cursor_bob_thresholds[];
extern battle_menu_cursor_bob_threshold_t g_battle_cursor_bob_active_thresholds[];

/* Return the cursor's vertical animation offset for the current menu frame.
 *
 * The two signed threshold tables produce the idle and active cursor curves. */
s32 battle_menu_get_cursor_bob_offset(s32 mode) {
    battle_menu_cursor_bob_threshold_t* entry;
    s32 counter;
    s32 threshold;

    counter = g_battle_thread_contexts[g_battle_thread_current_id].task_words[0];
    if (mode == 0) {
        entry = g_battle_cursor_bob_thresholds;
        counter %= entry->threshold;
    } else if (mode == 1) {
        entry = g_battle_cursor_bob_active_thresholds;
    }

    /* Any other mode leaves entry unset. The first threshold is tested here
     * and again by the loop condition. */
    entry++;
    threshold = entry->threshold;
    if (threshold == 0) {
        return 0;
    }
    while (threshold != 0) {
        if (counter < threshold) {
            return entry->offset;
        }
        entry++;
        threshold = entry->threshold;
    }
    return 0;
}
