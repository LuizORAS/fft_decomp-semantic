#include "fft/battle.h"
#include "psx/types.h"

/* Return the first other running thread, from slot 1, whose task id is task_id, or 0 when none is. */
s32 battle_thread_find_running_by_task(s32 task_id) {
    s32 thread_id;

    thread_id = 1;
    do {
        if (thread_id != g_battle_thread_current_id && battle_thread_is_running_2(thread_id) != 0
            && g_battle_thread_task_ids[thread_id][0] == task_id) {
            return thread_id;
        }
        thread_id += 1;
    } while (thread_id < 0x10);
    return 0;
}
