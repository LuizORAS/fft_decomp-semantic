#include "fft/thread.h"

/* Set thread_id's running flag again, so the scheduler continues it from its last yield. */
void thread_resume(s32 thread_id) {
    g_threads[thread_id].is_running = 1;
}
