#include "fft/thread.h"
#include "fft/world.h"

/* WORLD twin of battle_menu_wait_thread. */
void world_menu_wait_thread(void) {
    world_menu_entry_t* entry = (world_menu_entry_t*)world_thread_get_current_parameter_1();

    g_world_menu_wait_row_actions[0] = -2;
    g_world_menu_wait_row_actions[1] = -2;
    world_menu_confirm_action_silently(entry);
    thread_exit_current();
}
