#include "fft/world.h"
#include "psx/types.h"

/* Waits until neither companion load request is pending, publishes
 * `request` and yields until the loader clears it. The calling thread then
 * exits when g_world_frame_result_override is 1. */
void world_menu_request_open_companion_executable(s32 request) {
    while (g_world_bin_load_request != 0 || g_world_pending_bin_load_index != 0) {
        world_thread_yield();
    }
    g_world_bin_load_request = request;
    while (g_world_bin_load_request != 0) {
        world_thread_yield();
    }
    if (g_world_frame_result_override == 1) {
        world_thread_exit_current();
    }
}
