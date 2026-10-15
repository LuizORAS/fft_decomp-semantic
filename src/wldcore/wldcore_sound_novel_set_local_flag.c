#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 23: set local flag operand 0 (g_wldcore_sound_novel_local_flags). */
void wldcore_sound_novel_set_local_flag(void) {
    wldcore_bits_assign(g_wldcore_sound_novel_local_flags, g_wldcore_sound_novel_record.instruction.bytes.operand_0, 1);
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
