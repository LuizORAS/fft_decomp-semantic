#include "fft/wldcore.h"

/* Location script opcode 32 (DrawPath): end the script with a route to draw between two locations
 * (the map path level). */
void wldcore_script_request_draw_path(void) {
    g_wldcore_script_state.args[0] = g_wldcore_script_state.data[g_wldcore_script_state.ip];
    g_wldcore_script_state.ip++;
    g_wldcore_script_state.args[1] = g_wldcore_script_state.data[g_wldcore_script_state.ip];
    g_wldcore_script_state.ip++;
    g_wldcore_script_state.flags |= WLDCORE_SCRIPT_REQUEST_DRAW_PATH;
}
