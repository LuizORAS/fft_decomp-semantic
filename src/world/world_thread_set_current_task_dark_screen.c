#include "fft/world.h"
#include "psx/types.h"

/* Set the current thread's task id to NATIVE_THREAD_TASK_DARK_SCREEN. */
void world_thread_set_current_task_dark_screen(void) {
    world_thread_set_current_task_id(NATIVE_THREAD_TASK_DARK_SCREEN);
}
