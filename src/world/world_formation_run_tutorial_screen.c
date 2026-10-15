#include "fft/world.h"
#include "psx/types.h"

/* Run the formation screen with formation tutorial menu_index + 1, the row picked in WLDCORE's
 * list of formation tutorials (wldcore_list_handle_formation_tutorials_input), resetting the menu
 * context around it. */
void world_formation_run_tutorial_screen(s32 menu_index) {
    world_menu_reset_context();
    world_formation_run_screen(menu_index + 1, 0);
    world_menu_reset_context();
    g_world_menu_screen_open_request = 0;
    g_world_menu_thread_menu_data = g_world_menu_default_entries;
}
