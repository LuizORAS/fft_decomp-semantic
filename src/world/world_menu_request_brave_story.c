#include "fft/script.h"
#include "fft/thread.h"
#include "fft/world.h"

/* Thread entry of the world menu's Brave Story row (g_world_menu_default_entries[11]): start the
 * Brave Story request (world_menu_update_brave_story_request), hand the menu frame's input state to
 * the event input and exit the thread. */
void world_menu_request_brave_story(void) {
    g_world_brave_story_request_state = WORLD_SCREEN_REQUEST_START_FADE_OUT;
    g_world_menu_frame_new_buttons = 0;
    script_load_event_input_state(&g_world_menu_frame_input_state);
    thread_exit_current();
}
