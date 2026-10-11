#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound-novel opcode: subtract the 16-bit operand (instruction bits 8..23)
 * from the record's counter delta. */
void wldcore_opcode_subtract_counter_delta(void) {
    s16* target;
    u32 word;
    u16 flags;

    target = &g_wldcore_active_saved_record.counter_delta;
    word = g_wldcore_opcode_state.instruction;
    flags = g_wldcore_active_saved_record.state_flags;
    *target -= (word << 8) >> 16;
    g_wldcore_active_saved_record.state_flags = flags | WLDCORE_OPCODE_STATE_FETCH;
}
