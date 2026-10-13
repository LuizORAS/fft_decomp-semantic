#include "fft/thread.h"

void thread_resume(s32 thread_id) {
    g_threads[thread_id].is_running = 1;
}
