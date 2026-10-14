#include "fft/wldcore.h"
#include "fft/world.h"
#include "psx/types.h"

/* Step the Brave Story request (g_world_brave_story_request_state): fade the world map out, then
 * reset the menu context and raise MAIN_SYSTEM_FLAG_REQUEST_BRAVE_STORY, which WLDCORE's return level
 * (wldcore_menu_step_world_menu_level) turns into the Brave Story screen. */
void world_menu_update_brave_story_request(void) {
    s16 state = g_world_brave_story_request_state;

    if (state == WORLD_SCREEN_REQUEST_IDLE) {
        return;
    }
    if (state == WORLD_SCREEN_REQUEST_START_FADE_OUT) {
        g_world_menu_screen_open_request = 1;
        wldcore_gfx_start_screen_fade(0x12, 0x10);
        g_world_brave_story_request_state++;
        return;
    }
    if (state == WORLD_SCREEN_REQUEST_WAIT_FADE_OUT) {
        if (!(g_main_system_flags & MAIN_SYSTEM_FLAG_FADING)) {
            g_world_brave_story_request_state = state + 1;
        }
        return;
    }
    if (state == WORLD_SCREEN_REQUEST_RUN) {
        world_menu_reset_context();
        g_world_brave_story_request_state = WORLD_SCREEN_REQUEST_IDLE;
        g_main_system_flags |= MAIN_SYSTEM_FLAG_REQUEST_BRAVE_STORY;
    }
}
