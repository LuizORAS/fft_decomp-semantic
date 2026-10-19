#include "fft/wldcore.h"

/* Pop a proposition's last-attempt text (wldcore_menu_push_proposition_attempt_detail_level) once
 * its render record has finished popping in and the text window has finished: show the Work history
 * list's windows and render record again, pop the level, start the list's pop-in and let
 * wldcore_list_open_proposition_history redraw it. Mirrors wldcore_menu_wait_text_then_pop_level. */
void wldcore_menu_close_proposition_attempt_detail_level(void) {
    s32* index = &g_wldcore_scrollable_text_window.render_record_index;
    s32 depth;

    if (g_wldcore_window_render_records[*index].flags & WLDCORE_DISPLAY_OBJECT_POP_IN) {
        return;
    }
    if (wldcore_text_is_window_finished(&g_wldcore_scrollable_text_window) != 0) {
        return;
    }
    depth = g_wldcore_menu_stack_depth;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 2].window_render.window_index].flags
        &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[depth - 2].window_render.render_index].flags
        &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_menu_stack_depth = depth - 1;
    g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[depth - 2].window_render.render_index].flags
        |= WLDCORE_DISPLAY_OBJECT_POP_IN;
    wldcore_list_open_proposition_history();
}
