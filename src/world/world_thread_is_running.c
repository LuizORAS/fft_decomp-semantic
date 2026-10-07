#include "fft/world.h"

/* Return thread_id's running flag. */
s32 world_thread_is_running(s32 thread_id) {
    return g_world_threads[thread_id].is_running;
}
