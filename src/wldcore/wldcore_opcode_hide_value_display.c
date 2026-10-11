#include "fft/wldcore.h"
#include "psx/types.h"

/* Opcode 29: take the numeric value display down again (wldcore_opcode_start_pending_value_transition
 * puts it up) and fetch the next instruction. */
void wldcore_opcode_hide_value_display(void) {
    u16* flags;
    u16 value;

    flags = &g_wldcore_active_saved_record.state_flags;
    value = *flags;
    g_main_system_flags |= MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE;
    *flags = (value & ~WLDCORE_OPCODE_STATE_VALUE_DISPLAY) | WLDCORE_OPCODE_STATE_FETCH;
}
