#include "fft/thread.h"
#include "psx/types.h"

/* Set the current thread's task id (thread_task_e), which other threads find with
 * battle_thread_find_running_by_task (world_thread_find_running_by_task in WORLD's copy). */
void thread_set_current_task_id(s32 task_id) {
    g_thread_contexts[g_thread_current_id].task_id = task_id;
}
