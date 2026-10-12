#include "fft/world.h"
#include "psx/types.h"

/* Thread body of DarkScreen (EVENT_OPCODE_DARK_SCREEN). WORLD has no REQUIRE.OUT to run (BATTLE's
 * battle_script_dark_screen_thread runs its bonus image burst), so it only sets the current thread's
 * task id to NATIVE_THREAD_TASK_DARK_SCREEN. */
void world_script_dark_screen_thread(void) {
    world_thread_set_current_task_id(NATIVE_THREAD_TASK_DARK_SCREEN);
}
