#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 19: branch when local flag operand 0 is set
 * (wldcore_sound_novel_branch_if_local_flag). */
void wldcore_sound_novel_branch_if_local_flag_set(void) {
    wldcore_sound_novel_branch_if_local_flag(1);
}
