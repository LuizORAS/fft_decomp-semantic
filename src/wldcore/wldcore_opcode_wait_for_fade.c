#include "fft/wldcore.h"

/* Sound-novel opcode: yield while a screen fade is running. */
void wldcore_opcode_wait_for_fade(void) {
    if ((g_main_system_flags & MAIN_SYSTEM_FLAG_FADING) == 0) {
        g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_FETCH;
    } else {
        g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_YIELD;
    }
}
