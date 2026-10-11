#include "fft/wldcore.h"
#include "psx/types.h"

/* Script branch on a save-data bit selected by a 16-bit operand: when the
 * bit equals the expected value the branch target byte is armed at 0x800d484e
 * with state flag 0x02, otherwise only flag 0x04 is raised. */
void wldcore_opcode_branch_if_savedata_bit(s32 expected) {
    u32* instruction = &g_wldcore_opcode_state.instruction;
    u16 flags;

    if (wldcore_test_bit(g_main_saved_data_bits, ((u8)(*instruction >> 8) << 8) | (u8)(*instruction >> 16))
        == expected) {
        flags = g_wldcore_active_saved_record.state_flags | WLDCORE_OPCODE_STATE_BRANCH;
        g_wldcore_active_saved_record.section = (u8)(*instruction >> 24);
    } else {
        flags = g_wldcore_active_saved_record.state_flags | WLDCORE_OPCODE_STATE_FETCH;
    }
    g_wldcore_active_saved_record.state_flags = flags;
}
