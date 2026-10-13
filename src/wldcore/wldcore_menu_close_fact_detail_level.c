#include "fft/wldcore.h"

/* Step a fact's text window until it has finished scrolling, then pop the level, restore the Brave
 * Story list's windows and frame render record, and reopen the Facts list. */
void wldcore_menu_close_fact_detail_level(void) {
    s32 depth;

    if (!(g_wldcore_window_render_records[g_wldcore_scrollable_text_window.render_record_index].flags
            & WLDCORE_DISPLAY_OBJECT_POP_IN)
        && wldcore_text_is_window_finished(&g_wldcore_scrollable_text_window) == 0) {
        depth = g_wldcore_menu_stack_depth;
        g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[depth - 2].list_window.frame_render].flags
            |= WLDCORE_DISPLAY_OBJECT_POP_IN;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 2].list_window.main_window].flags
            &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 2].list_window.side_window].flags
            &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[depth - 2].list_window.frame_render].flags
            &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 2].list_window.upper_window].flags
            &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_menu_stack_depth = depth - 1;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 2].list_window.lower_window].flags
            &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        wldcore_list_open_facts();
    }
}
