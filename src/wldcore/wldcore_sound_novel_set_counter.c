#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 25: set the record's counter to the 16-bit operand (instruction bits 8..23). */
void wldcore_sound_novel_set_counter(void) {
    g_wldcore_sound_novel_record.counter = (g_wldcore_sound_novel_state.instruction.word << 8) >> 16;
    g_wldcore_sound_novel_state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
