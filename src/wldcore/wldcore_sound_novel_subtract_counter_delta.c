#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 27: subtract the 16-bit operand (instruction bits 8..23) from the record's
 * counter delta. */
void wldcore_sound_novel_subtract_counter_delta(void) {
    s16* target;
    u32 word;
    u16 flags;

    target = &g_wldcore_sound_novel_record.counter_delta;
    word = g_wldcore_sound_novel_state.instruction.word;
    flags = g_wldcore_sound_novel_record.state_flags;
    *target -= (word << 8) >> 16;
    g_wldcore_sound_novel_record.state_flags = flags | WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
