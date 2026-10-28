#include "fft/wldcore.h"

/* Sound novel opcode 16: branch when section operand 1 of novel operand 0 has not been read
 * (wldcore_sound_novel_branch_if_section_read_bit). */
void wldcore_sound_novel_branch_if_section_unread(void) {
    wldcore_sound_novel_branch_if_section_read_bit(0);
}
