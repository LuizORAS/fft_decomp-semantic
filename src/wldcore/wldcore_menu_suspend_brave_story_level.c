#include "fft/wldcore.h"

/* Suspend handler of the Brave Story menu (level BRAVE_STORY): the cursor window takes CLUT 10 and the
 * pressed cursor, the side window CLUT 6, the frame text CLUT 2 and the upper and lower windows CLUT 5.
 * While the help shows (WLDCORE_LIST_WINDOW_MODE_FADE_OR_HELP) the section's content takes CLUT 2;
 * otherwise the content and the value display (MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE) hide while a
 * section other than Person opens. wldcore_menu_handle_brave_story_input calls it too.
 *
 * The chained store keeps one main-window index for both cleared fields. */
void wldcore_menu_suspend_brave_story_level(wldcore_menu_list_window_level_t* state) {
    g_wldcore_window_records[state->main_window].palette = 10;
    g_wldcore_window_records[state->side_window].palette = 6;
    g_wldcore_window_render_records[state->frame_render].palette = 2;
    g_wldcore_window_records[state->upper_window].palette = 5;
    g_wldcore_window_records[state->lower_window].palette = 5;
    g_wldcore_window_records[state->main_window].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_PRESS;
    g_wldcore_window_records[state->main_window].frame_index = g_wldcore_window_records[state->main_window].anim_counter
        = 0;
    if (state->mode == WLDCORE_LIST_WINDOW_MODE_FADE_OR_HELP) {
        g_wldcore_window_render_records[state->content_render].palette = 2;
        return;
    }
    if (state->entries[state->selected_entry] != WLDCORE_BRAVE_STORY_ROW_PERSONS) {
        g_main_system_flags |= MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE;
        g_wldcore_window_render_records[state->content_render].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    }
}
