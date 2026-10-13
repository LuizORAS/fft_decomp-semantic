#include "fft/world.h"
#include "psx/types.h"

s32 world_thread_get_current_task_id(void) {
    return g_world_thread_contexts[g_world_thread_current_id].task_id;
}
