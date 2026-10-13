#include "fft/world.h"

void world_thread_set_current_task_id(s32 task_id) {
    g_world_thread_contexts[g_world_thread_current_id].task_id = task_id;
}
