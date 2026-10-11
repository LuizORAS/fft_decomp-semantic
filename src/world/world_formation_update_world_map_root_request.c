#include "fft/wldcore.h"
#include "fft/world.h"
#include "psx/types.h"

/* Sibling of world_formation_update_tutorial_categories_request driven by g_world_map_root_request_state. */
void world_formation_update_world_map_root_request(void) {
    s16 state = g_world_map_root_request_state;

    if (state == WORLD_SCREEN_REQUEST_IDLE) {
        return;
    }
    if (state == WORLD_SCREEN_REQUEST_START_FADE_OUT) {
        g_world_menu_screen_open_request = 1;
        wldcore_fade_start_screen(0x12, 0x10);
        g_world_map_root_request_state++;
        return;
    }
    if (state == WORLD_SCREEN_REQUEST_WAIT_FADE_OUT) {
        if (!(g_main_system_flags & MAIN_SYSTEM_FLAG_FADING)) {
            g_world_map_root_request_state = state + 1;
        }
        return;
    }
    if (state == WORLD_SCREEN_REQUEST_RUN) {
        world_formation_reset_menu_context();
        g_world_map_root_request_state = WORLD_SCREEN_REQUEST_IDLE;
        g_main_system_flags |= MAIN_SYSTEM_FLAG_REQUEST_BRAVE_STORY;
    }
}
