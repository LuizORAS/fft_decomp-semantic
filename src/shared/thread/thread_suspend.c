#include "fft/thread.h"

/* Clear thread_id's running flag; the scheduler skips the slot until thread_resume. */
void thread_suspend(s32 thread_id) {
    g_threads[thread_id].is_running = 0;
}
