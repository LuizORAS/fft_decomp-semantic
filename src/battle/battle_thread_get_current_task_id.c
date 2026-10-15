#include "fft/battle.h"
#include "psx/types.h"

/* Return the current thread's task id. */
s32 battle_thread_get_current_task_id(void) {
    return g_battle_thread_task_ids[g_battle_thread_current_id][0];
}
