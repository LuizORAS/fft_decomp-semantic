#include "fft/thread.h"
#include "fft/world.h"

/* End the current thread: mark its slot idle with no task and yield for the
 * last time. */
void world_thread_exit_current(void) {
    g_world_threads[g_world_thread_current_id].is_running = 0;
    g_world_threads[g_world_thread_current_id].task_id = NATIVE_THREAD_TASK_NONE;
    thread_yield();
}
