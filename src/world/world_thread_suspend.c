#include "fft/world.h"

/* Clear thread_id's running flag; the scheduler skips the slot until world_thread_resume. */
void world_thread_suspend(s32 thread_id) {
    g_world_threads[thread_id].is_running = 0;
}
