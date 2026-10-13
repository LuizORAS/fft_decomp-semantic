#include "fft/wldcore.h"

/* Suspend handler of Professor Daravon's tutorial menu (level TUTORIAL_CATEGORIES): the cursor window
 * takes CLUT 10 and the pressed cursor, the side window CLUT 6 and the text CLUT 2.
 * wldcore_list_handle_tutorial_categories_input calls it when a category opens.
 *
 * The record stores may alias `level`, so they invalidate the cached parameter loads and reproduce the
 * target's reloads; `index` shares one address computation between the last two. */
void wldcore_menu_suspend_tutorial_categories_level(wldcore_menu_list_window_level_t* level) {
    s32 index;

    g_wldcore_window_records[level->main_window].palette = 10;
    g_wldcore_window_records[level->side_window].palette = 6;
    g_wldcore_window_render_records[level->frame_render].palette = 2;
    g_wldcore_window_records[level->main_window].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_PRESS;
    index = level->main_window;
    g_wldcore_window_records[index].anim_counter = 0;
    g_wldcore_window_records[index].frame_index = 0;
}
