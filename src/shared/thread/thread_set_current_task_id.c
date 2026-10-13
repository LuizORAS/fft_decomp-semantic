#include "fft/thread.h"
#include "psx/types.h"

void thread_set_current_task_id(s32 task_id) {
    g_thread_contexts[g_thread_current_id].task_id = task_id;
}
