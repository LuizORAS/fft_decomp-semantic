#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_request_open_companion_data_file. */
void world_menu_request_open_companion_data_file(s32 request) {
    while (
        g_world_menu_companion_executable_request_index != 0 || g_world_menu_companion_data_file_request_index != 0) {
        world_thread_yield();
    }
    g_world_menu_companion_data_file_request_index = request;
    while (g_world_menu_companion_data_file_request_index != 0) {
        world_thread_yield();
    }
    if (g_world_script_frame_result_override == 1) {
        world_thread_exit_current();
    }
}
