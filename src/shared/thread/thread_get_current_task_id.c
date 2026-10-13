#include "fft/thread.h"

s32 thread_get_current_task_id(void) {
    return g_native_thread_contexts[g_native_current_thread_id].task_id;
}
