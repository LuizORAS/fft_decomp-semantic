#include "fft/thread.h"

/* End the current thread: mark its slot idle with no task, then yield. */
void thread_exit_current(void) {
    g_threads[g_thread_current_id].is_running = 0;
    g_threads[g_thread_current_id].task_id = THREAD_TASK_NONE;
    thread_yield();
}
