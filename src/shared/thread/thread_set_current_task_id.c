#include "fft/thread.h"
#include "psx/types.h"

void thread_set_current_task_id(s32 task_id) {
    g_native_thread_contexts[g_native_current_thread_id].task_id = task_id;
}
