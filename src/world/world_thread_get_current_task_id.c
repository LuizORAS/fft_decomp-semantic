#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

s32 world_thread_get_current_task_id(void) {
    return g_native_thread_contexts[g_native_current_thread_id].task_id;
}
