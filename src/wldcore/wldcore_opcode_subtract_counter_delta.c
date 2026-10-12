#include "fft/wldcore.h"

/* Sound-novel opcode: subtract the 16-bit operand (instruction bits 8..23)
 * from the record's counter delta. */
void wldcore_opcode_subtract_counter_delta(void) {
    g_wldcore_active_saved_record.counter_delta -= (g_wldcore_opcode_state.instruction << 8) >> 16;
    g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_FETCH;
}
