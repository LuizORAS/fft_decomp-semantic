#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound-novel opcode: set the record's counter to the 16-bit operand
 * (instruction bits 8..23). */
void wldcore_opcode_set_counter(void) {
    u16 flags;

    flags = g_wldcore_state_flags;
    g_wldcore_active_saved_record.counter = (g_wldcore_opcode_state.instruction << 8) >> 16;
    g_wldcore_state_flags = flags | WLDCORE_OPCODE_STATE_FETCH;
}
