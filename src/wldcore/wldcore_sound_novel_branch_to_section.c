#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 6: branch to section operand 0. */
void wldcore_sound_novel_branch_to_section(void) {
    g_wldcore_sound_novel_record.section = g_wldcore_sound_novel_record.instruction.bytes.operand_0;
    g_wldcore_sound_novel_state_flags |= WLDCORE_SOUND_NOVEL_STATE_BRANCH;
}
