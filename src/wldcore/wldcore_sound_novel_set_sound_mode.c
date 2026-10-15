#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 33: record sound mode operand 0 in the record. */
void wldcore_sound_novel_set_sound_mode(void) {
    u16 flags;

    flags = g_wldcore_sound_novel_state_flags | WLDCORE_SOUND_NOVEL_STATE_FETCH;
    g_wldcore_sound_novel_record.sound_mode = g_wldcore_sound_novel_record.instruction.bytes.operand_0;
    g_wldcore_sound_novel_state_flags = flags;
}
