#include "fft/wldcore.h"

/* Sound novel opcode 52: yield while a screen fade is running. */
void wldcore_sound_novel_wait_for_fade(void) {
    if ((g_main_system_flags & MAIN_SYSTEM_FLAG_FADING) == 0) {
        g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
    } else {
        g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_YIELD;
    }
}
