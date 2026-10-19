#include "fft/thread.h"
#include "fft/world.h"

/* Thread of the world menu's Options row (g_world_menu_default_entries[5]): run the game options
 * (world_option_game_options_thread) in the slot below with this thread's menu entry, wait for it and
 * exit. */
void world_option_open_game_options(void) {
    s32 thread_id;

    thread_start(g_thread_current_id - 1, world_option_game_options_thread);
    thread_id = g_thread_current_id;
    thread_set_parameters(thread_id - 1, g_threads[thread_id].function_parameter_1, 0, 0);
    thread_wait_until_inactive(g_thread_current_id - 1);
    thread_exit_current();
}
