#include "fft/thread.h"
#include "psx/types.h"

/* Return the first other running thread in slots 1-16 whose task id is task_id, or 0 when none is. */
s32 world_thread_find_running_by_task(s32 task_id) {
    s32 thread_id;

    for (thread_id = 1; thread_id < 17; thread_id++) {
        if (thread_id != g_thread_current_id && world_thread_is_running_asm(thread_id) != 0
            && g_thread_contexts[thread_id].task_id == task_id) {
            return thread_id;
        }
    }
    return 0;
}
