#include "fft/battle.h"
#include "psx/types.h"

/* Set the current thread's task id (native_thread_task_e), which other threads find with
 * battle_thread_find_running_by_task. */
void battle_thread_set_current_task_id(s32 task_id) {
    g_battle_thread_task_ids[g_battle_thread_current_id][0] = task_id;
}
