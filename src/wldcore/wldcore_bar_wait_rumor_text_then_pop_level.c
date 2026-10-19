#include "fft/wldcore.h"

/* Step of the rumor text level: once the scrollable text window has finished (the wait of
 * wldcore_menu_wait_text_then_pop_level), show the Bar's windows again, pop the level and reopen the
 * rumor list. */
void wldcore_bar_wait_rumor_text_then_pop_level(wldcore_menu_rumor_detail_level_t* level) {
    s32* index = &g_wldcore_scrollable_text_window.render_record_index;

    if (g_wldcore_window_render_records[*index].flags & WLDCORE_DISPLAY_OBJECT_POP_IN) {
        return;
    }
    if (wldcore_text_is_window_finished(&g_wldcore_scrollable_text_window) != 0) {
        return;
    }
    g_wldcore_window_records[level->first_window].flags ^= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_records[level->second_window].flags ^= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_render_records[level->render_index].flags ^= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_menu_stack_depth--;
    g_wldcore_window_render_records[level->render_index].flags |= WLDCORE_DISPLAY_OBJECT_POP_IN;
    wldcore_bar_open_rumors();
}
