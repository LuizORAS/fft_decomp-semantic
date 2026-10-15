#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 18: branch when saved flag operand 0 is clear
 * (wldcore_sound_novel_branch_if_saved_flag). */
void wldcore_sound_novel_branch_if_saved_flag_clear(void) {
    wldcore_sound_novel_branch_if_saved_flag(0);
}
