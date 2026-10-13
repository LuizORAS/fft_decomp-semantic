#include "fft/menu.h"
#include "fft/thread.h"
#include "fft/world.h"

/* Per-frame driver for menu_request_open_companion_executable: start the disc read for the
 * pending request on the main thread, then poll until it finishes and clear
 * the request. */
void world_menu_open_companion_executable(void) {
    s32 request;

    request = g_menu_companion_executable_request_index;
    if (request != 0 && g_world_bin_load_in_progress == 0) {
        g_thread_call_target = (void (*)(void))main_file_call_build_header;
        if (thread_call_on_main_stack(g_world_bin_load_sectors[request], g_world_bin_load_sizes[request],
                g_world_bin_load_address_offsets[request] + (s32)g_main_heap_companion_overlay_load_address)
            == 0) {
            g_world_bin_load_in_progress = 1;
        }
    } else if (g_menu_companion_executable_request_index != 0 && g_world_bin_load_in_progress != 0) {
        g_thread_call_target = main_file_is_still_loading;
        if (thread_call_on_main_stack() == 0) {
            g_world_bin_load_in_progress = 0;
            g_menu_companion_executable_request_index = 0;
        }
    }
}
