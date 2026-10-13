#include "fft/thread.h"

void thread_suspend(s32 thread_id) {
    g_threads[thread_id].is_running = 0;
}
