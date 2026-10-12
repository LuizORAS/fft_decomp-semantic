#include "fft/wldcore.h"
#include "psx/types.h"

/* Location script opcode 26 (Choice): end the script with four variable and value pairs, the
 * choices the location's menu offers (wldcore_location_collect_choice_text_ids). */
void wldcore_script_request_choice(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        g_wldcore_script_state.choice_vars[i] = g_wldcore_script_state.data[g_wldcore_script_state.ip];
        g_wldcore_script_state.args[i] = g_wldcore_script_state.data[++g_wldcore_script_state.ip];
        g_wldcore_script_state.ip++;
    }
    g_wldcore_script_state.flags |= WLDCORE_SCRIPT_REQUEST_CHOICE;
}
