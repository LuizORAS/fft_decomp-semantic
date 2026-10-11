#include "fft/world.h"
#include "psx/types.h"

/* Return g_world_thread_task_active. */
s32 world_thread_is_task_active(s32 unused_0) {
    return g_world_thread_task_active;
}
