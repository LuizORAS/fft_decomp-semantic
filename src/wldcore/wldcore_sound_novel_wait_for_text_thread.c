#include "fft/thread.h"
#include "fft/wldcore.h"

/* Sound novel opcode 4: yield while the message thread is running. */
void wldcore_sound_novel_wait_for_text_thread(void) {
    if (thread_is_running(WLDCORE_THREAD_SLOT_MESSAGE) == 0) {
        g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
    } else {
        g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_YIELD;
    }
}
