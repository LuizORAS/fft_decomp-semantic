#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound-novel opcode: branch to one of the three sections named by the
 * operand bytes, chosen by rand() with equal weight. */
void wldcore_opcode_branch_to_random_section(void) {
    s32 pick;
    s32 value;

    pick = (rand() * 3) >> 15;
    if (pick == 0) {
        value = (u8)(g_wldcore_active_saved_record.instruction >> 8);
    }
    if (pick == 1) {
        value = (u8)(g_wldcore_active_saved_record.instruction >> 16);
    }
    if (pick == 2) {
        value = (u8)(g_wldcore_active_saved_record.instruction >> 24);
    }
    g_wldcore_active_saved_record.section = value;
    g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_BRANCH;
}
