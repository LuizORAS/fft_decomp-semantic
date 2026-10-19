#include "fft/thread.h"
#include "psx/types.h"

/* Return the first other running thread, from slot 1, whose task id is task_id, or 0 when none is. */
s32 battle_thread_find_running_by_task(s32 task_id) {
    s32 thread_id;

    for (thread_id = 1; thread_id < THREAD_SLOT_COUNT; thread_id++) {
        if (thread_id != g_thread_current_id && battle_thread_is_running_asm(thread_id) != 0
            && g_thread_contexts[thread_id].task_id == task_id) {
            return thread_id;
        }
    }
    return 0;
}
