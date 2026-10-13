#include "fft/thread.h"

s32 world_thread_is_running(s32 thread_id) {
    return g_native_threads[thread_id].is_running;
}
