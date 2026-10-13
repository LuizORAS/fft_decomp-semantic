#include "fft/thread.h"

/* Return the running flag of a scheduler slot. */
s32 thread_is_running(s32 thread_id) {
    return g_threads[thread_id].is_running;
}
