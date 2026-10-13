#include "fft/battle_thread.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Yield until thread_id's slot is idle. It yields before the first test. */
void battle_thread_wait_until_inactive(s32 thread_id) {
    do {
        thread_yield();
    } while (g_native_threads[thread_id].is_running != 0);
}
