#include "fft/menu.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_open_companion_data_file. */
void world_menu_open_companion_data_file(void) {
    s32 idx = g_menu_companion_data_file_request_index;

    if (idx != 0 && g_world_menu_companion_data_file_loading == 0) {
        g_thread_call_target = (void (*)(void))main_file_request_read_bytes;
        if (thread_call_on_main_stack(g_world_menu_companion_data_file_lbas[idx],
                g_world_menu_companion_data_file_sizes[idx], g_world_pending_bin_load_destination)
            == 0) {
            g_world_menu_companion_data_file_loading = 1;
        }
    } else if (g_menu_companion_data_file_request_index != 0 && g_world_menu_companion_data_file_loading != 0) {
        g_thread_call_target = main_file_is_still_loading;
        if (thread_call_on_main_stack() == 0) {
            g_world_menu_companion_data_file_loading = 0;
            g_menu_companion_data_file_request_index = 0;
        }
    }
}
