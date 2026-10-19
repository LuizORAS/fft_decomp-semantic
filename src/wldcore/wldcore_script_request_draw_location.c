#include "fft/wldcore.h"

/* Location script opcode 34 (DrawLocation): end the script with a location's dot to show (the
 * location visibility level). It does not step past its operand; the script ends there anyway. */
void wldcore_script_request_draw_location(void) {
    g_wldcore_script_flags |= WLDCORE_SCRIPT_REQUEST_DRAW_LOCATION;
    g_wldcore_script_state.args[0] = g_wldcore_script_state.data[g_wldcore_script_state.ip];
}
