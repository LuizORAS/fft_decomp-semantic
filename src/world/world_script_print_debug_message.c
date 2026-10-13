#include "fft/thread.h"
#include "fft/world.h"
#include "psx/libgpu.h"

/* Print the script pause message, yield one frame and set the debug-paused
 * flag. */
void world_script_print_debug_message(void) {
    FntPrint(g_world_script_pause_message);
    thread_yield();
    g_world_script_debug_paused = 1;
}
