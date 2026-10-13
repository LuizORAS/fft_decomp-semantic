#include "fft/thread.h"
#include "fft/world.h"

/* Park every idle thread slot from 5 to 8 on world_thread_idle_yield_forever. */
void world_thread_start_idle_5_to_8(void) {
    s32 thread_id = 8;

    do {
        thread_t* thread = &g_native_threads[thread_id];

        if (thread->is_running == 0) {
            thread_start(thread_id, world_thread_idle_yield_forever);
        }
        thread_id--;
    } while (thread_id >= 5);
}
