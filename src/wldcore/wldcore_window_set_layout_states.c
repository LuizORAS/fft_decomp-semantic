#include "fft/wldcore.h"
#include "psx/types.h"

/* Publish the window/render states for one world-map layout: four 36-byte
 * window records and one 52-byte render record, each selected by an index
 * word of the caller's descriptor. */
void wldcore_window_set_layout_states(s32* param) {
    g_wldcore_window_records[param[0]].palette = 10;
    g_wldcore_window_records[param[1]].palette = g_wldcore_window_records[param[2]].palette
        = g_wldcore_window_records[param[3]].palette = 6;
    g_wldcore_window_render_records[param[4]].palette = 2;
}
