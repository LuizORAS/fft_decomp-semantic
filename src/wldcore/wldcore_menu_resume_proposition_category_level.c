#include "fft/wldcore.h"

/* Resume handler of the Job section: its cursor window and text and the Brave Story menu's content go
 * back to CLUT 0, the cursor to its idle sequence, and the Brave Story's content and the value display
 * come back (popping in) if hidden. */
void wldcore_menu_resume_proposition_category_level(wldcore_menu_list_window_level_t* level) {
    g_wldcore_window_records[level->main_window].palette = g_wldcore_window_render_records[level->side_window].palette
        = g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 2]
                                              .list_window.content_render]
              .palette = 0;
    g_wldcore_window_records[level->main_window].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_IDLE;
    g_wldcore_window_records[level->main_window].frame_index = g_wldcore_window_records[level->main_window].anim_counter
        = 0;
    if (g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 2]
                                            .list_window.content_render]
            .flags
        & WLDCORE_DISPLAY_OBJECT_HIDDEN) {
        g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 2]
                                            .list_window.content_render]
            .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_main_system_flags &= ~MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE;
        g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 2]
                                            .list_window.content_render]
            .flags |= WLDCORE_DISPLAY_OBJECT_POP_IN;
    }
}
