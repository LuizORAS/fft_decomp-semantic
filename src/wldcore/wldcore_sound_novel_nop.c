#include "fft/wldcore.h"

/* Sound novel opcode 2: do nothing and fetch the next instruction. */
void wldcore_sound_novel_nop(void) {
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
