#include "fft/wldcore.h"
#include "psx/types.h"

void wldcore_opcode_branch_to_section(void) {
    u16 flags;

    flags = g_wldcore_state_flags | WLDCORE_OPCODE_STATE_BRANCH;
    g_wldcore_active_saved_record.section = (u8)(g_wldcore_active_saved_record.instruction >> 8);
    g_wldcore_state_flags = flags;
}
