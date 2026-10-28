#include "fft/wldcore.h"

/* Sound novel opcode 26: add the 16-bit operand (instruction bits 8..23) to the record's counter
 * delta. */
void wldcore_sound_novel_add_counter_delta(void) {
    g_wldcore_sound_novel_record.counter_delta += (g_wldcore_sound_novel_state.instruction.word << 8) >> 16;
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
