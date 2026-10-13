#include "fft/world.h"

/* WORLD twin of battle_menu_open_companion_executable. */
void world_menu_open_companion_executable(void) {
    s32 request;

    request = g_world_menu_companion_executable_request_index;
    if (request != 0 && g_world_menu_companion_executable_loading == 0) {
        g_world_thread_call_target = (void (*)(void))main_file_request_read_bytes;
        if (world_thread_call_on_main_stack(g_world_menu_companion_executable_lbas[request],
                g_world_menu_companion_executable_sizes[request],
                g_world_menu_companion_executable_offsets[request] + (s32)g_main_heap_companion_overlay_load_address)
            == 0) {
            g_world_menu_companion_executable_loading = 1;
        }
    } else if (g_world_menu_companion_executable_request_index != 0 && g_world_menu_companion_executable_loading != 0) {
        g_world_thread_call_target = main_file_is_still_loading;
        if (world_thread_call_on_main_stack() == 0) {
            g_world_menu_companion_executable_loading = 0;
            g_world_menu_companion_executable_request_index = 0;
        }
    }
}
