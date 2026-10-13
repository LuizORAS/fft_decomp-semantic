#include "fft/battle_thread.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Zeroes all 16 thread records (16 * 0x400 bytes), makes thread 0 the current
 * thread and marks it running. */
void battle_thread_reset_scheduler(void) {
    s32* word;
    s32 index;

    index = (NATIVE_THREAD_ARRAY_BYTES / sizeof(s32)) - 1;
    word = (s32*)((u8*)g_native_threads + NATIVE_THREAD_LAST_WORD_OFFSET);
    do {
        *word = 0;
        index -= 1;
        word -= 1;
    } while (index >= 0);
    g_native_current_thread_id = NATIVE_THREAD_SLOT_MAIN;
    g_native_threads[NATIVE_THREAD_SLOT_MAIN].is_running = 1;
}
