#include "fft/wldcore.h"

/* Sound-novel opcode: add the 16-bit operand (instruction bits 8..23) to the
 * record's counter delta. */
void wldcore_opcode_add_counter_delta(void) {
    s16* target;

    target = &g_wldcore_active_saved_record.counter_delta;
    *target += (g_wldcore_opcode_state.instruction << 8) >> 16;
    g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_FETCH;
}
