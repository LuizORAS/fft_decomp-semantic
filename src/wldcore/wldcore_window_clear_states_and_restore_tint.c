#include "fft/wldcore.h"
#include "psx/types.h"

/* Clears the palette of two window records and two render records, resets one
 * render record's colour to neutral grey, then clears two more windows. */
void wldcore_window_clear_states_and_restore_tint(wldcore_window_tint_request_t* param) {
    wldcore_window_entry_52_rgb_t* colors;

    g_wldcore_window_records[param->window_a].palette = g_wldcore_window_render_records[param->render_a].palette
        = g_wldcore_window_render_records[param->render_b].palette = g_wldcore_window_records[param->window_b].palette
        = 0;
    colors = (wldcore_window_entry_52_rgb_t*)&g_wldcore_window_render_records->red;
    colors[param->render_color].red = 0x80;
    colors[param->render_color].green = 0x80;
    colors[param->render_color].blue = 0x80;
    g_wldcore_window_records[param->window_c].palette = g_wldcore_window_records[param->window_d].palette = 0;
}
