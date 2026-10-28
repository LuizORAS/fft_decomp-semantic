#include "fft/wldcore.h"

/* Resume handler of the Quit / Continue prompt: its windows and text go back to CLUT 0 and the cursor
 * to its idle sequence.
 *
 * The record stores may alias `level`, so they invalidate the cached parameter loads and reproduce the
 * target's reloads between stores. The two leading locals are what hoist the first pair of loads above
 * the first store, and `index` shares one address computation between the last two. `scratch` is
 * unreferenced; it reproduces the target's 8-byte leaf frame, which saves no registers and holds no
 * spills. */
void wldcore_menu_resume_sound_novel_quit_level(wldcore_menu_stack_record_t* level) {
    s32 first = level->sound_novel_quit.cursor_window;
    s32 second = level->sound_novel_quit.second_window;
    s32 index;
    /* The original frame reserves eight unused bytes. */
    s32 scratch[2];

    g_wldcore_window_render_records[level->sound_novel_quit.render_index].palette = 0;
    g_wldcore_window_records[second].palette = 0;
    g_wldcore_window_records[first].palette = 0;
    g_wldcore_window_records[level->sound_novel_quit.cursor_window].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_IDLE;
    index = level->sound_novel_quit.cursor_window;
    g_wldcore_window_records[index].anim_counter = 0;
    g_wldcore_window_records[index].frame_index = 0;
}
