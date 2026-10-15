#include "fft/world.h"
#include "psx/types.h"

/* Return the current thread's task id. */
s32 world_thread_get_current_task_id(void) {
    return g_world_thread_task_ids[g_world_thread_current_id][0];
}
