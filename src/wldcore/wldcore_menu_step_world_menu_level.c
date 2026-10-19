#include "fft/script.h"
#include "fft/wldcore.h"

/* Step of the WORLD menu level while WORLD's menu is open. After any fade and a 3-frame delay it
 * captures the map behind the menu (wldcore_gfx_toggle_captured_world_frame) and unblocks the
 * buttons; a screen clear in between (MAIN_SYSTEM_FLAG_SCREEN_CLEARED) shows the map again and
 * restarts the delay. When WORLD's menu threads have ended it shows the map, pops the level and acts
 * on what the menu asked:
 *  - Brave Story, the tutorials, the debug variable list or the Party number panel open that screen;
 *  - a destination (MAIN_SYSTEM_FLAG_REQUEST_DESTINATION, from the location list) captures the map
 *    again, resumes the root level and travels to EVENT_SCRIPT_VAR_CURRENT_MAP;
 *  - a loaded game (MAIN_SYSTEM_FLAG_RESTART, set by the card menu) ends the visit: a new visit
 *    starts when the loaded game has an event waiting for the formation or no next scenario,
 *    otherwise wldcore_entrypoint returns to start the scenario;
 *  - otherwise the root level resumes. */
void wldcore_menu_step_world_menu_level(wldcore_menu_world_menu_level_t* level) {
    s32 flags;
    s32 cleared;
    s32 location;

    if (g_main_system_flags & MAIN_SYSTEM_FLAG_SCREEN_CLEARED) {
        level->pending = 1;
    }
    if (g_main_system_flags & MAIN_SYSTEM_FLAG_FADING) {
        if (level->pending != 0) {
            level->pending = 0;
            wldcore_gfx_toggle_captured_world_frame(0);
            level->delay = 3;
            g_main_system_flags |= MAIN_SYSTEM_FLAG_FADING | MAIN_SYSTEM_FLAG_FREEZE_ANIMATION;
        }
        g_wldcore_menu_blocked_buttons = -1;
        return;
    }
    if (level->delay != 0) {
        level->delay -= 1;
        if (level->delay == 0) {
            wldcore_gfx_toggle_captured_world_frame(1);
        }
        return;
    }
    g_wldcore_menu_blocked_buttons = 0;
    if (world_menu_get_thread_activity() != 0) {
        return;
    }
    wldcore_input_init_repeat_state();
    flags = g_main_system_flags;
    wldcore_gfx_toggle_captured_world_frame(0);
    cleared = g_main_system_flags & ~MAIN_SYSTEM_FLAG_FREEZE_ANIMATION;
    g_main_system_flags = cleared;
    g_wldcore_menu_stack_depth -= 1;
    if (flags & MAIN_SYSTEM_FLAG_REQUEST_BRAVE_STORY) {
        g_main_system_flags = cleared ^ MAIN_SYSTEM_FLAG_DRAW_MAP;
        wldcore_menu_push_screen_transition_level(WLDCORE_MENU_LEVEL_BRAVE_STORY, 0, 0);
        return;
    }
    if (flags & MAIN_SYSTEM_FLAG_REQUEST_DESTINATION) {
        location = script_get_variable(EVENT_SCRIPT_VAR_CURRENT_MAP);
        wldcore_gfx_capture_map_backdrop();
        wldcore_menu_dispatch_resume_handler();
        if (g_wldcore_map_projection_state.marker.kind != location) {
            wldcore_map_start_travel(g_wldcore_map_projection_state.marker.kind, location);
        }
        return;
    }
    if (flags & MAIN_SYSTEM_FLAG_REQUEST_TUTORIAL) {
        g_main_system_flags = cleared ^ MAIN_SYSTEM_FLAG_DRAW_MAP;
        wldcore_menu_push_screen_transition_level(WLDCORE_MENU_LEVEL_TUTORIAL_CATEGORIES, 0, 1);
        return;
    }
    if (flags & MAIN_SYSTEM_FLAG_REQUEST_VARIABLE_LIST) {
        wldcore_menu_push_variable_list_level();
        return;
    }
    if (flags & MAIN_SYSTEM_FLAG_REQUEST_PARTY_NUMBER_PANEL) {
        wldcore_menu_push_party_number_panel_level();
        return;
    }
    if (flags & MAIN_SYSTEM_FLAG_RESTART) {
        g_main_system_flags = cleared & ~(MAIN_SYSTEM_FLAG_RUNNING | MAIN_SYSTEM_FLAG_UNLOCK_ALL);
        if (script_get_variable(EVENT_SCRIPT_VAR_FORMATION_RETURN_EVENT) != 0
            || script_get_variable(EVENT_SCRIPT_VAR_NEXT_SCENARIO) == 0) {
            g_main_system_flags |= MAIN_SYSTEM_FLAG_RESTART;
        } else {
            g_main_system_flags &= ~MAIN_SYSTEM_FLAG_RESTART;
        }
        g_main_system_flags |= MAIN_SYSTEM_FLAG_NO_BACKGROUND_IMAGE;
        return;
    }
    wldcore_menu_dispatch_resume_handler();
}
