#include "fft/world.h"
#include "psx/types.h"

/* Reset the listed menu entries' selections and start the system-function
 * worker thread with `parameter` as its first parameter. */
void world_menu_start_system_function_thread(s32 parameter) {
    world_menu_clear_entry_selections();
    world_thread_start(NATIVE_THREAD_SLOT_SYSTEM_FUNCTION, world_menu_run_system_function_thread);
    world_thread_set_parameters(NATIVE_THREAD_SLOT_SYSTEM_FUNCTION, parameter, 0, 0);
}
