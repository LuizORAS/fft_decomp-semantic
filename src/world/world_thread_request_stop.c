#include "fft/world.h"

/* Store NATIVE_THREAD_TASK_STOP_REQUEST in thread_id's task id, asking a text thread to
 * stop; WLDCORE's sound novel screens send it to thread 14. */
void world_thread_request_stop(s32 thread_id) {
    native_thread_t* thread = &g_world_threads[thread_id];

    thread->task_id = NATIVE_THREAD_TASK_STOP_REQUEST;
}
