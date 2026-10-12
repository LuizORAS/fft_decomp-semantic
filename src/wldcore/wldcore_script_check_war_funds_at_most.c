#include "fft/wldcore.h"
#include "psx/types.h"

/* Location script opcode 15 (Gil<=): fail the script when the war funds are above the operand. */
void wldcore_script_check_war_funds_at_most(void) {
    u16* ip = &g_wldcore_script_state.ip;
    u16 idx = *ip;
    s32 value = g_wldcore_script_state.data[idx];
    *ip = idx + 1;
    if (value < world_script_get_variable(EVENT_SCRIPT_VAR_WAR_FUNDS)) {
        g_wldcore_script_state.flags |= WLDCORE_SCRIPT_RESULT_CONDITION_FAILED;
    }
}
