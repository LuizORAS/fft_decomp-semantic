#include "fft/script.h"
#include "fft/wldcore.h"
#include "psx/types.h"

/* Branches the sound-novel script to section operand_0 when the
 * selected dialog option equals operand byte 2 (or differs from it, when `branch_if_equal` is 0); otherwise raises
 * state flag 4. */
void wldcore_opcode_branch_if_dialog_option(s32 branch_if_equal) {
    u8 operand = (u8)(g_wldcore_active_saved_record.instruction >> 16);

    if (operand == script_get_variable(EVENT_SCRIPT_VAR_SELECTED_DIALOG_OPTION) ? branch_if_equal == 1
                                                                                : branch_if_equal == 0) {
        g_wldcore_active_saved_record.section = (u8)(g_wldcore_active_saved_record.instruction >> 8);
        g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_BRANCH;
    } else {
        u16* flags = &g_wldcore_active_saved_record.state_flags;

        *flags |= WLDCORE_OPCODE_STATE_FETCH;
    }
}
