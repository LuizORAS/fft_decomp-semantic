#include "fft/wldcore.h"
#include "psx/types.h"

/* Suspend handler of the send-unit list (level PROPOSITION_SEND_UNIT): its two windows take CLUTs 6
 * and 10, both texts CLUT 2, and one render record is tinted blue (0x40, 0x50, 0x70).
 * wldcore_proposition_step_send_unit_level calls it too. */
void wldcore_menu_suspend_proposition_send_unit_level(wldcore_window_tint_request_t* param) {
    s32 pad[4];

    g_wldcore_window_records[param->window_a].palette = 6;
    g_wldcore_window_render_records[param->render_a].palette = 2;
    g_wldcore_window_render_records[param->render_b].palette = 2;
    g_wldcore_window_records[param->window_b].palette = 10;
    g_wldcore_window_render_records[param->render_color].color.r = 0x40;
    g_wldcore_window_render_records[param->render_color].color.g = 0x50;
    g_wldcore_window_render_records[param->render_color].color.b = 0x70;
}
