#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 52: yield while a screen fade is running. */
void wldcore_sound_novel_wait_for_fade(void) {
    u16* flags;

    if ((g_main_system_flags & MAIN_SYSTEM_FLAG_FADING) == 0) {
        flags = &g_wldcore_sound_novel_record.state_flags;
        *flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
    } else {
        flags = &g_wldcore_sound_novel_record.state_flags;
        *flags |= WLDCORE_SOUND_NOVEL_STATE_YIELD;
    }
}
