#include "fft/thread.h"

/* Zeroes all 16 thread records (16 * 0x400 bytes), makes the main thread
 * current and marks it running. */
void world_thread_reset_scheduler(void) {
    s32* word;
    s32 i;

    /* Clear the whole 16 x 0x400 thread array, last 32-bit value first. */
    word = (s32*)((u8*)g_threads + THREAD_LAST_WORD_OFFSET);
    i = (THREAD_ARRAY_BYTES / sizeof(s32)) - 1;
    do {
        *word = 0;
        i--;
        word--;
    } while (i >= 0);
    g_thread_current_id = THREAD_SLOT_MAIN;
    g_threads->is_running = 1;
}
