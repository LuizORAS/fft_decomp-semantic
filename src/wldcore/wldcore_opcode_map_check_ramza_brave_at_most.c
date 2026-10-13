#include "fft/wldcore.h"

/* Reject the MCBrave<= condition when Ramza's Brave is above the operand. */
void wldcore_opcode_map_check_ramza_brave_at_most(void) {
    party_unit_t* party;
    u16 ip;
    s32 val;

    party = wldcore_get_party_unit(wldcore_get_ramza_roster_index());
    ip = g_wldcore_script_state.ip;
    val = g_wldcore_script_state.data[ip];
    g_wldcore_script_state.ip = ip + 1;
    if (val < party->bravery) {
        g_wldcore_script_state.flags |= WLDCORE_SCRIPT_RESULT_CONDITION_FAILED;
    }
}
