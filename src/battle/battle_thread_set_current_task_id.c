#include "fft/battle_thread.h"
#include "psx/types.h"

void battle_thread_set_current_task_id(s32 task_id) {
    g_battle_thread_contexts[g_battle_current_thread_id].task_id = task_id;
}
