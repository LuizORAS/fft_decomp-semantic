#include "fft/wldcore.h"

/* Stage the text ID consumed by the deferred world-script text request. */
void wldcore_opcode_map_request_text(void) {
    g_wldcore_script_state.args[0] = g_wldcore_script_state.data[g_wldcore_script_state.ip];
    g_wldcore_script_state.ip++;
    g_wldcore_script_state.flags |= WLDCORE_SCRIPT_REQUEST_TEXT;
}
