#include "fft/thread.h"

/* Stop the current thread (clear its running flag and task id) and yield; it runs again only
 * if its slot is resumed or restarted. */
void thread_exit_current(void) {
    g_threads[g_thread_current_id].is_running = 0;
    g_threads[g_thread_current_id].task_id = THREAD_TASK_NONE;
    thread_yield();
}
