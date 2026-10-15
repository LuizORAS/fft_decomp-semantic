#include "fft/wldcore.h"
#include "psx/types.h"

/* Resume handler of the send-unit list: its windows and texts go back to CLUT 0 and the tinted render
 * record to neutral grey (0x80), and two more windows go back to CLUT 0. */
void wldcore_menu_resume_proposition_send_unit_level(wldcore_window_tint_request_t* param) {
    wldcore_window_entry_52_rgb_t* colors;

    g_wldcore_window_records[param->window_a].palette = g_wldcore_window_render_records[param->render_a].palette
        = g_wldcore_window_render_records[param->render_b].palette = g_wldcore_window_records[param->window_b].palette
        = 0;
    colors = (wldcore_window_entry_52_rgb_t*)&g_wldcore_window_render_records->color;
    colors[param->render_color].red = 0x80;
    colors[param->render_color].green = 0x80;
    colors[param->render_color].blue = 0x80;
    g_wldcore_window_records[param->window_c].palette = g_wldcore_window_records[param->window_d].palette = 0;
}
