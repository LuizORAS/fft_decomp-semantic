#include "fft/battle.h"
#include "psx/types.h"

/* Clear the 16 thread slots and the current id, and mark slot 0, the main loop, running.
 * battle_menu_reset_subsystems calls it. */
void battle_thread_reset_scheduler(void) {
    s32* word;
    s32 index;

    index = (NATIVE_THREAD_ARRAY_BYTES / sizeof(s32)) - 1;
    word = (s32*)((u8*)g_battle_threads + NATIVE_THREAD_LAST_WORD_OFFSET);
    do {
        *word = 0;
        index -= 1;
        word -= 1;
    } while (index >= 0);
    g_battle_thread_current_id = 0;
    g_battle_threads[0].is_running = 1;
}
