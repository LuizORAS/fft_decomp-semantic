#include "fft/thread.h"
#include "fft/world.h"

/* Same as thread_is_running; a byte-identical copy at 0x80112c6c. */
s32 world_thread_is_running_by_id(s32 thread_id) {
    thread_t* thread = &g_threads[thread_id];

    return thread->is_running;
}
