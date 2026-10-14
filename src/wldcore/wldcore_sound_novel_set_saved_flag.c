#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 21: set saved flag operand 0 (g_main_sound_novel_saved_flags). */
void wldcore_sound_novel_set_saved_flag(void) {
    wldcore_bits_assign(g_main_sound_novel_saved_flags, g_wldcore_sound_novel_record.instruction.bytes.operand_0, 1);
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
