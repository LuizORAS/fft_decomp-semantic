#include "fft/wldcore.h"

void wldcore_opcode_wait_for_text_thread(void) {
    if (world_thread_is_running(WORLD_THREAD_SLOT_WLDCORE_MESSAGE) == 0) {
        g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_FETCH;
    } else {
        g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_YIELD;
    }
}
