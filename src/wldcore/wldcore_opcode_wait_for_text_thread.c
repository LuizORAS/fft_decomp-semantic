#include "fft/thread.h"
#include "fft/wldcore.h"

/* Sound-novel opcode: yield while the message thread is running. */
void wldcore_opcode_wait_for_text_thread(void) {
    if (thread_is_running(WLDCORE_THREAD_SLOT_MESSAGE) == 0) {
        g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_FETCH;
    } else {
        g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_YIELD;
    }
}
