#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 14: branch to one of the three sections named by the operand bytes, chosen by
 * rand() with equal weight. */
void wldcore_sound_novel_branch_to_random_section(void) {
    s32 pick;
    s32 value;

    pick = (rand() * 3) >> 15;
    if (pick == 0) {
        value = g_wldcore_sound_novel_record.instruction.bytes.operand_0;
    }
    if (pick == 1) {
        value = g_wldcore_sound_novel_record.instruction.bytes.operand_1;
    }
    if (pick == 2) {
        value = g_wldcore_sound_novel_record.instruction.bytes.operand_2;
    }
    g_wldcore_sound_novel_record.section = value;
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_BRANCH;
}
