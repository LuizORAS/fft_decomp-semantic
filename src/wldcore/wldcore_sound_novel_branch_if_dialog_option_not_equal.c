#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 8: branch to section operand 0 when the dialog option chosen differs from operand
 * 1 (wldcore_sound_novel_branch_if_dialog_option). */
void wldcore_sound_novel_branch_if_dialog_option_not_equal(void) {
    wldcore_sound_novel_branch_if_dialog_option(0);
}
