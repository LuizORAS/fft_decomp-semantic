#include "fft/world.h"
#include "psx/types.h"

/* Thread entry of the world menu's tutorial row (g_world_menu_default_entries[14]): start the
 * tutorial categories request (world_menu_update_tutorial_categories_request), hand the menu frame's
 * input state to the event input and exit the thread. */
void world_menu_request_tutorial_categories(void) {
    g_world_tutorial_categories_request_state = WORLD_SCREEN_REQUEST_START_FADE_OUT;
    g_world_menu_frame_new_buttons = 0;
    world_script_load_event_input_state(&g_world_menu_frame_input_state);
    world_thread_exit_current();
}
