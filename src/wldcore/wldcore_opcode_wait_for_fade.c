#include "fft/wldcore.h"

/* Sound-novel opcode: yield while a screen fade is running. */
void wldcore_opcode_wait_for_fade(void) {
    u16* flags;

    if ((g_main_system_flags & MAIN_SYSTEM_FLAG_FADING) == 0) {
        flags = &g_wldcore_active_saved_record.state_flags;
        *flags |= WLDCORE_OPCODE_STATE_FETCH;
    } else {
        flags = &g_wldcore_active_saved_record.state_flags;
        *flags |= WLDCORE_OPCODE_STATE_YIELD;
    }
}
