#include "fft/wldcore.h"
#include "psx/types.h"

/* Suspend handler of the sound novel's Quit / Continue prompt (level SOUND_NOVEL_QUIT): the cursor
 * window takes CLUT 10 and the still cursor, the side window CLUT 6 and the text CLUT 2.
 * wldcore_menu_step_sound_novel_quit_level calls it before Select opens a row's help.
 *
 * The record stores may alias `level`, so they invalidate the cached parameter loads, which reproduces the
 * target's reloads between stores. */
void wldcore_menu_suspend_sound_novel_quit_level(wldcore_menu_sound_novel_quit_level_t* level) {
    s32 index;

    g_wldcore_window_records[level->cursor_window].palette = 10;
    g_wldcore_window_records[level->second_window].palette = 6;
    g_wldcore_window_render_records[level->render_index].palette = 2;
    g_wldcore_window_records[level->cursor_window].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_STILL;
    index = level->cursor_window;
    g_wldcore_window_records[index].anim_counter = 0;
    g_wldcore_window_records[index].frame_index = 0;
}
