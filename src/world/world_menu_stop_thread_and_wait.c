#include "fft/world.h"
#include "psx/types.h"

/* Ask a thread to stop and run event input frames until it has ended; then
 * clear the menu-thread running status. */
void world_menu_stop_thread_and_wait(s32 thread_id) {
    u32 unused[2];

    while (world_thread_is_running(thread_id) != 0) {
        world_menu_request_thread_stop(thread_id);
        world_input_update_event_state(unused, 0, 0);
    }
    g_world_menu_thread_running_status = 0;
}
