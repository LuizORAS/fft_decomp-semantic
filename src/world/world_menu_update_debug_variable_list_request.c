#include "fft/world.h"
#include "psx/types.h"

/* Serve a pending request to open the debug variable list from the world map: reset the menu
 * context and raise MAIN_SYSTEM_FLAG_REQUEST_VARIABLE_LIST, which WLDCORE's return level turns into
 * the variable list. */
void world_menu_update_debug_variable_list_request(void) {
    s32 state = g_world_debug_variable_list_request_state;

    if (state != 0 && state == 1) {
        g_world_menu_screen_open_request = 1;
        world_menu_reset_context();
        world_menu_set_brightness(0x80, 0x80, 0x80);
        g_world_debug_variable_list_request_state = 0;
        g_main_system_flags |= MAIN_SYSTEM_FLAG_REQUEST_VARIABLE_LIST;
    }
}
