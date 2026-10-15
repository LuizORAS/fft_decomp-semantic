#include "fft/wldcore.h"

/* Location script opcode 29 (Text): end the script with the text the location shows on arrival. */
void wldcore_script_request_text(void) {
    g_wldcore_script_state.args[0] = g_wldcore_script_state.data[g_wldcore_script_state.ip];
    g_wldcore_script_state.ip++;
    g_wldcore_script_state.flags |= WLDCORE_SCRIPT_REQUEST_TEXT;
}
