#include "fft/wldcore.h"
#include "psx/types.h"

/* Clears the palette of two window records and two render records, resets one
 * render record's colour to neutral grey, then clears two more windows. */
void wldcore_window_clear_states_and_restore_tint(wldcore_window_tint_request_t* param) {
    CVECTOR* colors;

    g_wldcore_window_records[param->window_a].palette = g_wldcore_window_render_records[param->render_a].palette
        = g_wldcore_window_render_records[param->render_b].palette = g_wldcore_window_records[param->window_b].palette
        = 0;
    /* Direct record.color access changes the column base and instruction scheduling. */
    colors = &g_wldcore_window_render_records->color;
    ((CVECTOR*)((u8*)colors + param->render_color * sizeof(g_wldcore_window_render_records[0])))->r = 0x80;
    ((CVECTOR*)((u8*)colors + param->render_color * sizeof(g_wldcore_window_render_records[0])))->g = 0x80;
    ((CVECTOR*)((u8*)colors + param->render_color * sizeof(g_wldcore_window_render_records[0])))->b = 0x80;
    g_wldcore_window_records[param->window_c].palette = g_wldcore_window_records[param->window_d].palette = 0;
}
