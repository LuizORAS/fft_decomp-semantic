#include "fft/wldcore.h"

/* Sound novel opcode 11: hide the picture's render record and forget the picture. */
void wldcore_sound_novel_hide_picture(void) {
    g_wldcore_window_render_records[g_wldcore_sound_novel_record.picture_render_index].flags
        |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_sound_novel_record.picture_id = -1;
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH | WLDCORE_SOUND_NOVEL_STATE_YIELD;
}
