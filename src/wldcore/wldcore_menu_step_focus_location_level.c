#include "fft/wldcore.h"

/* Step of the focus location level (WLDCORE_MENU_LEVEL_FOCUS_LOCATION): while the camera pans, it
 * advances the pan and redraws the map; then it pops the level, puts the parent's cursor window on
 * the dot (4 pixels above it), shows the location label again and resumes the parent.
 *
 * The record must be a struct pointer: the target hoists its reload above the
 * scalar g_main_system_flags store, which GCC only allows for an in-struct
 * access. */
void wldcore_menu_step_focus_location_level(wldcore_menu_stack_record_t* record) {
    wldcore_projection_state_t* state;
    s32 level;

    if (g_wldcore_map_projection_motion.flags & WLDCORE_PROJECTION_MOTION_ACTIVE) {
        wldcore_map_step_projection_motion();
        state = &g_wldcore_map_projection_state;
        state->flags |= WLDCORE_PROJECTION_FLAG_UPDATE_POSITION;
        g_main_system_flags |= MAIN_SYSTEM_FLAG_DRAW_MAP;
        return;
    }
    level = g_wldcore_menu_stack_depth;
    g_wldcore_menu_stack_depth = level - 1;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[level - 2].window_index].position.x
        = g_wldcore_map_dots[record->focus_dot_index].render.position.x;
    g_main_system_flags &= ~MAIN_SYSTEM_FLAG_HIDE_LOCATION_LABEL;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[level - 2].window_index].position.y
        = g_wldcore_map_dots[record->focus_dot_index].render.position.y - 4;
    wldcore_menu_dispatch_resume_handler();
}
