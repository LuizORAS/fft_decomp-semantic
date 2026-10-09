#include "fft/wldcore.h"

/* Clears the window/render states for one world-map layout: the palette word
 * of four 36-byte window records and one 52-byte render record, each selected
 * by an index word of the caller's descriptor. Sibling of
 * wldcore_window_set_layout_states, which writes the same fields with
 * non-zero values. */
void wldcore_window_clear_layout_states(s32* param) {
    g_wldcore_window_records[param[0]].palette = g_wldcore_window_records[param[1]].palette
        = g_wldcore_window_records[param[2]].palette = g_wldcore_window_records[param[3]].palette
        = g_wldcore_window_render_records[param[4]].palette = 0;
    g_wldcore_window_records[param[0]].sequence = 2;
    g_wldcore_window_records[param[0]].frame_index = g_wldcore_window_records[param[0]].anim_counter = 0;
}
