#include "fft/script.h"
#include "fft/thread.h"
#include "fft/world.h"

/* Menu thread body with no window of its own: with the menu sounds muted,
 * every frame it handles a Circle confirm of row 0 and a Cross cancel of its
 * thread group, until world_menu_check_thread_completion reports the menu
 * finished. It exits
 * unless thread parameter 2 is set. */
void world_menu_input_loop_thread(void) {
    menu_entry_t* parameter = (menu_entry_t*)world_thread_get_current_parameter_1();

    g_world_menu_sound_muted = 1;
    for (;;) {
        thread_yield();
        if (world_menu_check_thread_completion(&g_script_event_input) != 0) {
            break;
        }
        world_menu_handle_entry_confirm(parameter, 0);
        world_menu_cancel_thread_group(parameter);
    }
    thread_yield();
    g_world_menu_sound_muted = 0;
    if (world_thread_get_current_parameter_2() == 0) {
        thread_exit_current();
    }
}
