#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 17: branch when saved flag operand 0 is set
 * (wldcore_sound_novel_branch_if_saved_flag). */
void wldcore_sound_novel_branch_if_saved_flag_set(void) {
    wldcore_sound_novel_branch_if_saved_flag(1);
}
