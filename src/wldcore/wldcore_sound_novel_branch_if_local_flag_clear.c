#include "fft/wldcore.h"

/* Sound novel opcode 20: branch when local flag operand 0 is clear
 * (wldcore_sound_novel_branch_if_local_flag). */
void wldcore_sound_novel_branch_if_local_flag_clear(void) {
    wldcore_sound_novel_branch_if_local_flag(0);
}
