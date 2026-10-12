#include "fft/wldcore.h"

/* Location script opcode 31 (SetVar2): end the script with a deferred variable write, applied when
 * the party stands on the location without a story event: arriving there during travel, or setting
 * out from it. */
void wldcore_script_request_set_var_2(void) {
    g_wldcore_script_state.args[0] = g_wldcore_script_state.data[g_wldcore_script_state.ip];
    g_wldcore_script_state.ip++;
    g_wldcore_script_state.args[1] = g_wldcore_script_state.data[g_wldcore_script_state.ip];
    g_wldcore_script_state.ip++;
    g_wldcore_script_state.flags |= WLDCORE_SCRIPT_REQUEST_SET_VAR_2;
}
