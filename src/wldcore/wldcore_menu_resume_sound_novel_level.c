#include "fft/wldcore.h"
#include "psx/types.h"

/* Resume handler of the sound novel reader, once its Quit prompt or history closes: reload the
 * novel's background set (when it has one) and wait for it, show the picture again, show the two
 * windows named by the level when the novel keeps them shown
 * (WLDCORE_SOUND_NOVEL_STATE_PARENT_WINDOWS_VISIBLE), and resume WORLD thread 14, the novel's text. */
void wldcore_menu_resume_sound_novel_level(s32* param) {
    s16 set = g_wldcore_sound_novel_record.background_set;

    if (set != -1) {
        wldcore_gfx_load_background_set(set);
        wldcore_file_wait_for_load();
    }
    g_wldcore_window_render_records[g_wldcore_sound_novel_record.picture_render_index].flags
        &= ~WLDCORE_DISPLAY_OBJECT_BLANK;
    if (g_wldcore_sound_novel_record.state_flags & WLDCORE_SOUND_NOVEL_STATE_PARENT_WINDOWS_VISIBLE) {
        g_wldcore_window_records[param[0]].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_records[param[1]].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
    }
    world_thread_resume(WLDCORE_THREAD_SLOT_MESSAGE);
}
