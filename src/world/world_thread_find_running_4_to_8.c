#include "fft/world.h"

/* Return the first slot from 4 to 8 whose running flag is exactly 1, or 9 when none is.
 * The battle help menu uses it. */
s32 world_thread_find_running_4_to_8(void) {
    s32 thread_id;
    native_thread_t* thread;

    thread_id = 4;
    thread = &g_world_threads[4];
    do {
        if (thread->is_running == 1) {
            return thread_id;
        }
        thread_id++;
        thread++;
    } while (thread_id < 9);
    return thread_id;
}
