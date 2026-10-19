#include "fft/wldcore.h"

/* Per-frame step of a Tips entry's text (level TUTORIAL_TOPIC_TEXT): once the text window has popped in
 * and finished (wldcore_text_is_window_finished), pop the level with the cancel sound, bring the Tips
 * list's windows back and reopen it (wldcore_list_open_tutorial_topics). */
void wldcore_menu_wait_tutorial_topic_text_then_pop_level(void) {
    s32 depth;

    if (!(g_wldcore_window_render_records[g_wldcore_scrollable_text_window.render_record_index].flags
            & WLDCORE_DISPLAY_OBJECT_POP_IN)
        && wldcore_text_is_window_finished(&g_wldcore_scrollable_text_window) == 0) {
        wldcore_sound_play_effect(MAIN_SFX_CANCEL);
        depth = g_wldcore_menu_stack_depth;
        g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[depth - 2].window_pair_render.render_index]
            .flags |= WLDCORE_DISPLAY_OBJECT_POP_IN;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 2].window_pair_render.first_window].flags
            &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 2].window_pair_render.second_window].flags
            &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_menu_stack_depth = depth - 1;
        g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[depth - 2].window_pair_render.render_index]
            .flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        wldcore_list_open_tutorial_topics();
    }
}
