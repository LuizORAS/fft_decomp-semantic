#include "fft/wldcore.h"

/* Resume handler of the Advancing list: its windows and text go back to CLUT 0 and the cursor to its
 * idle sequence. */
void wldcore_menu_resume_active_propositions_level(s32* param) {
    g_wldcore_window_records[param[0]].palette = g_wldcore_window_records[param[1]].palette
        = g_wldcore_window_records[param[2]].palette = g_wldcore_window_records[param[3]].palette
        = g_wldcore_window_render_records[param[4]].palette = 0;
    g_wldcore_window_records[param[0]].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_IDLE;
    g_wldcore_window_records[param[0]].frame_index = g_wldcore_window_records[param[0]].anim_counter = 0;
}
