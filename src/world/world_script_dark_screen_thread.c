#include "fft/world.h"
#include "psx/types.h"

/* The thread EVENT_OPCODE_DARK_SCREEN starts. WORLD has no REQUIRE.OUT to run: it only takes the task id. */
void world_script_dark_screen_thread(void) {
    world_thread_set_current_task_id(NATIVE_THREAD_TASK_DARK_SCREEN);
}
