#include "fft/thread.h"
#include "fft/world.h"

/* Yield until the given thread slot is idle, at least once. */
void world_thread_wait_until_inactive(s32 thread_id) {
    do {
        thread_yield();
    } while (g_world_threads[thread_id].is_running != 0);
}
