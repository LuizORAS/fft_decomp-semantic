#include "fft/wldcore.h"

/* Hides the sound-novel picture's render record and forgets the picture. */
void wldcore_opcode_hide_picture(void) {
    g_wldcore_window_render_records[g_wldcore_active_saved_record.picture_render_index].flags
        |= WORLD_RENDER_RECORD_HIDDEN;
    g_wldcore_active_saved_record.picture_id = -1;
    g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_FETCH | WLDCORE_OPCODE_STATE_YIELD;
}
