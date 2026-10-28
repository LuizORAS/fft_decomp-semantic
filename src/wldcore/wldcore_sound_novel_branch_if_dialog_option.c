#include "fft/script.h"
#include "fft/wldcore.h"
#include "psx/types.h"

/* Branch to section operand 0 when the dialog option chosen (EVENT_SCRIPT_VAR_SELECTED_DIALOG_OPTION)
 * equals operand 1, or differs from it when `branch_if_equal` is 0; otherwise go on. Opcodes 7 and
 * 8. */
void wldcore_sound_novel_branch_if_dialog_option(s32 branch_if_equal) {
    u8 operand = g_wldcore_sound_novel_record.instruction.bytes.operand_1;

    if (operand == script_get_variable(EVENT_SCRIPT_VAR_SELECTED_DIALOG_OPTION) ? branch_if_equal == 1
                                                                                : branch_if_equal == 0) {
        g_wldcore_sound_novel_record.section = g_wldcore_sound_novel_record.instruction.bytes.operand_0;
        g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_BRANCH;
    } else {
        u16* flags = &g_wldcore_sound_novel_record.state_flags;

        *flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
    }
}
