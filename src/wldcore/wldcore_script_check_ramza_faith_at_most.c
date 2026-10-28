#include "fft/wldcore.h"

/* Location script opcode 40 (MCFaith<=): fail the script when Ramza's Faith is above the
 * operand. */
void wldcore_script_check_ramza_faith_at_most(void) {
    party_unit_t* party;
    u16 ip;
    s32 val;

    party = wldcore_unit_get_party_unit(wldcore_unit_get_ramza_roster_index());
    ip = g_wldcore_script_state.ip;
    val = g_wldcore_script_state.data[ip];
    g_wldcore_script_state.ip = ip + 1;
    if (val < party->faith) {
        g_wldcore_script_state.flags |= WLDCORE_SCRIPT_RESULT_CONDITION_FAILED;
    }
}
