#include "fft/wldcore.h"

/* Location script opcode 25 (LoadEvent): end the script with a story event: the event id and its
 * transition (args 0 and 1), which travel starts in place of moving on. */
void wldcore_script_request_load_event(void) {
    g_wldcore_script_state.args[0] = g_wldcore_script_state.data[g_wldcore_script_state.ip];
    g_wldcore_script_state.ip++;
    g_wldcore_script_state.args[1] = g_wldcore_script_state.data[g_wldcore_script_state.ip];
    g_wldcore_script_state.ip++;
    g_wldcore_script_state.flags |= WLDCORE_SCRIPT_REQUEST_LOAD_EVENT;
}
