#include "fft/world.h"

/* Set thread_id's running flag again, so the scheduler continues it from its last yield. */
void world_thread_resume(s32 thread_id) {
    g_world_threads[thread_id].is_running = 1;
}
