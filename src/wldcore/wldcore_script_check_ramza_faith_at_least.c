#include "fft/wldcore.h"

/* Location script opcode 39 (MCFaith>=): fail the script when Ramza's Faith is below the
 * operand. */
void wldcore_script_check_ramza_faith_at_least(void) {
    party_data_t* party;
    u16* ipp;
    u16 ip;
    s32 val;

    party = wldcore_unit_get_party_data_pointer(wldcore_unit_get_ramza_roster_index());
    ipp = &g_wldcore_script_state.ip;
    ip = *ipp;
    val = g_wldcore_script_state.data[ip];
    *ipp = ip + 1;
    if (party->faith < val) {
        g_wldcore_script_state.flags |= WLDCORE_SCRIPT_RESULT_CONDITION_FAILED;
    }
}
