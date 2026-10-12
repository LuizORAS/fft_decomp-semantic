#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 25: set the record's counter to the 16-bit operand (instruction bits 8..23). */
void wldcore_sound_novel_set_counter(void) {
    u32 word;
    u16 flags;

    word = g_wldcore_sound_novel_state.instruction.word;
    flags = g_wldcore_sound_novel_state_flags;
    g_wldcore_sound_novel_record.counter = (word << 8) >> 16;
    g_wldcore_sound_novel_state_flags = flags | WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
