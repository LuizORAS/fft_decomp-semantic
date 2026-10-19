#include "fft/wldcore.h"

/* Suspend handler of the Brave Story's Job section (level PROPOSITION_CATEGORY): the cursor window
 * takes CLUT 10 and the still cursor, the text CLUT 2, and the Brave Story menu's content (the level
 * underneath) and the value display hide. wldcore_menu_handle_proposition_category_input calls it
 * before a list opens. */
void wldcore_menu_suspend_proposition_category_level(wldcore_menu_list_window_level_t* level) {
    g_wldcore_window_records[level->main_window].palette = 10;
    g_wldcore_window_render_records[level->side_window].palette = 2;
    g_wldcore_window_records[level->main_window].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_STILL;
    g_wldcore_window_records[level->main_window].frame_index = g_wldcore_window_records[level->main_window].anim_counter
        = 0;
    g_main_system_flags |= MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE;
    g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth - 2]
                                        .list_window.content_render]
        .flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
}
