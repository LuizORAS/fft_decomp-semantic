#include "fft/wldcore.h"

/* Sound novel opcode 7: branch to section operand 0 when the dialog option chosen equals operand 1
 * (wldcore_sound_novel_branch_if_dialog_option). */
void wldcore_sound_novel_branch_if_dialog_option_equal(void) {
    wldcore_sound_novel_branch_if_dialog_option(1);
}
