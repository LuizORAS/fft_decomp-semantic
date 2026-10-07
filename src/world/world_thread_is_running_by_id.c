#include "fft/world.h"

/* Same as world_thread_is_running; a byte-identical copy at 0x80112c6c. */
s32 world_thread_is_running_by_id(s32 thread_id) {
    native_thread_t* thread = &g_world_threads[thread_id];

    return thread->is_running;
}
