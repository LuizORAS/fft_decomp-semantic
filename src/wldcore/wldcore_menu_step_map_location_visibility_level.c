#include "fft/wldcore.h"

/* Step of the location visibility level: once the screen fade has finished it plays the sound and,
 * to show the location, sets its visibility variable; then it waits 16 frames with the map
 * redrawing. Hiding a location clears its variable and hides its dot only at the end. Finally it
 * pops the level, shows the location label and the parent's window again and resumes the parent.
 *
 * The window index is read into a local before the flag clear, which keeps
 * the target's load and store order. */
void wldcore_menu_step_map_location_visibility_level(wldcore_map_visibility_level_t* level) {
    s32 index;

    if (level->pending_sound != 0) {
        if (g_main_system_flags & MAIN_SYSTEM_FLAG_FADING) {
            return;
        }
        if (level->erase != 0) {
            wldcore_sound_play_effect(MAIN_SFX_CANCEL);
        } else {
            world_script_set_variable(level->dot_index + EVENT_SCRIPT_VAR_LOCATION_VISIBLE_FIRST, 1);
            wldcore_sound_play_effect(MAIN_SFX_WORLD_MAP_LOCATION_APPEAR);
        }
        level->pending_sound = 0;
    }
    if (level->delay != 0) {
        g_main_system_flags |= MAIN_SYSTEM_FLAG_DRAW_MAP;
        level->delay--;
        return;
    }
    if (level->erase != 0) {
        g_wldcore_map_dots[level->dot_index].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        world_script_set_variable(level->dot_index + EVENT_SCRIPT_VAR_LOCATION_VISIBLE_FIRST, 0);
    }
    g_main_system_flags |= MAIN_SYSTEM_FLAG_DRAW_MAP;
    g_wldcore_menu_stack_depth--;
    index = g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 1].window_index;
    g_main_system_flags &= ~MAIN_SYSTEM_FLAG_HIDE_LOCATION_LABEL;
    g_wldcore_window_records[index].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
    wldcore_menu_dispatch_resume_handler();
}
