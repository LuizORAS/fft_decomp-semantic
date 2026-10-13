#include "fft/battle_thread.h"
#include "psx/types.h"

s32 battle_thread_get_current_task_id(void) {
    return g_battle_thread_contexts[g_battle_current_thread_id].task_id;
}
