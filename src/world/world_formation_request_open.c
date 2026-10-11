#include "fft/world.h"
#include "psx/types.h"

/* Thread entry of the world menu's formation row (g_world_menu_default_entries[7]): start the
 * formation screen request (world_formation_update_open_request), hand the menu frame's input state to
 * the event input and exit the thread. */
void world_formation_request_open(void) {
    g_world_formation_open_request_state = WORLD_SCREEN_REQUEST_START_FADE_OUT;
    g_world_menu_frame_new_buttons = 0;
    world_script_load_event_input_state(&g_world_menu_frame_input_state);
    world_thread_exit_current();
}
