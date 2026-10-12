#include "fft/world.h"
#include "psx/types.h"

/* Drives the file load requested through g_world_menu_companion_data_file_request_index: starts the disc read
 * on the main thread, then polls it until it finishes and clears the
 * request. */
void world_menu_open_companion_data_file(void) {
    s32 idx = g_world_menu_companion_data_file_request_index;

    if (idx != 0 && g_world_menu_companion_data_file_loading == 0) {
        g_world_thread_call_target = (void (*)(void))main_file_request_read_bytes;
        if (world_thread_call_on_main_stack(g_world_menu_companion_data_file_lbas[idx],
                g_world_menu_companion_data_file_sizes[idx], g_world_pending_bin_load_destination)
            == 0) {
            g_world_menu_companion_data_file_loading = 1;
        }
    } else if (g_world_menu_companion_data_file_request_index != 0 && g_world_menu_companion_data_file_loading != 0) {
        g_world_thread_call_target = main_file_is_still_loading;
        if (world_thread_call_on_main_stack() == 0) {
            g_world_menu_companion_data_file_loading = 0;
            g_world_menu_companion_data_file_request_index = 0;
        }
    }
}
