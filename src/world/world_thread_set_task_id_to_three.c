#include "fft/thread.h"
#include "fft/world.h"

/* Ask a thread to stop by setting its task id to
 * NATIVE_THREAD_TASK_STOP_REQUEST. */
void world_thread_set_task_id_to_three(s32 thread_id) {
    thread_t* thread = &g_native_threads[thread_id];

    thread->task_id = NATIVE_THREAD_TASK_STOP_REQUEST;
}
