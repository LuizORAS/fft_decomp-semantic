#include "fft/thread.h"
#include "fft/world.h"

/* Store THREAD_TASK_STOP_REQUEST in thread_id's task id, asking a text thread to
 * stop; WLDCORE's sound novel screens send it to thread 14. */
void world_thread_request_stop(s32 thread_id) {
    thread_t* thread = &g_threads[thread_id];

    thread->task_id = THREAD_TASK_STOP_REQUEST;
}
