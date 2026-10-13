#include "fft/thread.h"

/* Zeroes all 16 thread records (16 * 0x400 bytes), makes thread 0 the current
 * thread and marks it running. */
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
