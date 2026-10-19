#include "fft/world.h"
#include "psx/types.h"

/* Ask a menu thread to stop (world_menu_request_thread_stop) and clear the menu-thread running
 * status. */
void world_menu_stop_thread(s32 thread_id) {
    world_menu_request_thread_stop(thread_id);
    g_world_menu_thread_running_status = 0;
}
