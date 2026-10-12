#include "fft/wldcore.h"
#include "psx/types.h"

/* The double negation keeps the native counter/delta add operand order. */
void wldcore_opcode_branch_if_less(void) {
    s32 value;
    s32 operand;
    u32 word;

    value = g_wldcore_active_saved_record.counter - (-g_wldcore_active_saved_record.counter_delta);
    if (0xffff < value) {
        value = 0xffff;
    }
    if (value < 0) {
        value = 0;
    }
    word = g_wldcore_opcode_state.instruction;
    operand = (word >> 8) & 0xffff;
    if (value < operand) {
        g_wldcore_active_saved_record.section = word >> 24;
        g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_BRANCH;
    } else {
        g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_FETCH;
    }
}
