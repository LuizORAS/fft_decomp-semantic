#include "fft/world.h"
#include "psx/types.h"

/* Thread entry of g_world_menu_default_entries[17]: start the fixed number panel request
 * (world_menu_update_fixed_number_panel_request), hand the menu frame's input state to the event input
 * and exit the thread. */
void world_menu_request_fixed_number_panel(void) {
    g_world_fixed_number_panel_request_state = 1;
    g_world_menu_frame_new_buttons = 0;
    world_script_load_event_input_state(&g_world_menu_frame_input_state);
    world_thread_exit_current();
}
