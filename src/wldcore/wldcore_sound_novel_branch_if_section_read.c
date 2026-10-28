#include "fft/wldcore.h"

/* Sound novel opcode 15: branch when section operand 1 of novel operand 0 has been read
 * (wldcore_sound_novel_branch_if_section_read_bit). */
void wldcore_sound_novel_branch_if_section_read(void) {
    wldcore_sound_novel_branch_if_section_read_bit(1);
}
