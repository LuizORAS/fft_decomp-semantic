#include "fft/thread.h"

/* Clear slots 0-15 (16 * 0x400 bytes) and the current id, and mark slot 0, the main loop, running;
 * slot 16 keeps its state (QUIRKS.md). battle_menu_reset_subsystems calls it, and
 * world_menu_reset_subsystems WORLD's copy. */
void thread_reset_scheduler(void) {
    s32* word;
    s32 index;

    index = (THREAD_ARRAY_BYTES / sizeof(s32)) - 1;
    word = (s32*)((u8*)g_threads + THREAD_LAST_WORD_OFFSET);
    do {
        *word = 0;
        index -= 1;
        word -= 1;
    } while (index >= 0);
    g_thread_current_id = THREAD_SLOT_MAIN;
    g_threads[THREAD_SLOT_MAIN].is_running = 1;
}
