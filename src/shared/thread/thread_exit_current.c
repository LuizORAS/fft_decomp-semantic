#include "fft/thread.h"

/* End the current thread: mark its slot idle with no task, then yield. */
void thread_exit_current(void) {
    g_native_threads[g_native_current_thread_id].is_running = 0;
    g_native_threads[g_native_current_thread_id].task_id = NATIVE_THREAD_TASK_NONE;
    thread_yield();
}
