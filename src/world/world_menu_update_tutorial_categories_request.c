#include "fft/wldcore.h"
#include "fft/world.h"
#include "psx/types.h"

/* Step a pending request to open the tutorial categories from the world map: start the screen
 * fade-out, wait for it, then reset the menu context and raise MAIN_SYSTEM_FLAG_REQUEST_TUTORIAL, which
 * WLDCORE's return level (wldcore_menu_step_world_menu_level) turns into the tutorial menu. */
void world_menu_update_tutorial_categories_request(void) {
    s16 state = g_world_tutorial_categories_request_state;

    if (state == WORLD_SCREEN_REQUEST_IDLE) {
        return;
    }
    if (state == WORLD_SCREEN_REQUEST_START_FADE_OUT) {
        g_world_menu_screen_open_request = 1;
        wldcore_fade_start_screen(0x12, 0x10);
        g_world_tutorial_categories_request_state++;
        return;
    }
    if (state == WORLD_SCREEN_REQUEST_WAIT_FADE_OUT) {
        if (!(g_main_system_flags & MAIN_SYSTEM_FLAG_FADING)) {
            g_world_tutorial_categories_request_state = state + 1;
        }
        return;
    }
    world_menu_reset_context();
    world_menu_set_brightness(0x80, 0x80, 0x80);
    g_main_system_flags |= MAIN_SYSTEM_FLAG_REQUEST_TUTORIAL;
    g_world_tutorial_categories_request_state = WORLD_SCREEN_REQUEST_IDLE;
}
