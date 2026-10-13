#include "fft/script.h"
#include "fft/thread.h"
#include "fft/world.h"

/* Menu action: request the fixed number panel from the world map, hand the formation input
 * state to the event input and exit the thread. */
void world_formation_request_fixed_number_panel(void) {
    g_world_fixed_number_panel_request_state = 1;
    g_world_formation_menu_new_buttons = 0;
    script_load_event_input_state(&g_world_formation_menu_input_state);
    thread_exit_current();
}
