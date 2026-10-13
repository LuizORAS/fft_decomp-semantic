#include "fft/thread.h"
#include "fft/world.h"

s32 world_thread_is_running_by_id(s32 thread_id) {
    thread_t* thread = &g_threads[thread_id];

    return thread->is_running;
}
