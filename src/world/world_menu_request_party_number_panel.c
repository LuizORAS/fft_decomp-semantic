#include "fft/script.h"
#include "fft/thread.h"
#include "fft/world.h"

/* Thread entry of g_world_menu_default_entries[17]: start the Party number panel request
 * (world_menu_update_party_number_panel_request), hand the menu frame's input state to the event input
 * and exit the thread. */
void world_menu_request_party_number_panel(void) {
    g_world_party_number_panel_request_state = 1;
    g_world_menu_frame_new_buttons = 0;
    script_load_event_input_state(&g_world_menu_frame_input_state);
    thread_exit_current();
}
