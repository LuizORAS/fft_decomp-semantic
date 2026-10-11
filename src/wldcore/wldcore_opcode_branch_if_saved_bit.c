#include "fft/wldcore.h"
#include "psx/types.h"

/* Script branch on a bit of the second save-data 32-bit array selected by the
 * single operand byte: when the bit equals the expected value the branch
 * target byte is armed at 0x800d484e with state flag 0x02, otherwise only
 * flag 0x04 is raised. */
void wldcore_opcode_branch_if_saved_bit(s32 expected) {
    u32* instruction = &g_wldcore_opcode_state.instruction;
    u16 flags;

    /* Keep the instruction address live across the call for both operand loads. */
    if (wldcore_test_bit(g_main_secondary_saved_data_bits, (u8)(*instruction >> 8)) == expected) {
        flags = g_wldcore_active_saved_record.state_flags | WLDCORE_OPCODE_STATE_BRANCH;
        g_wldcore_active_saved_record.section = (u8)(*instruction >> 16);
    } else {
        flags = g_wldcore_active_saved_record.state_flags | WLDCORE_OPCODE_STATE_FETCH;
    }
    g_wldcore_active_saved_record.state_flags = flags;
}
