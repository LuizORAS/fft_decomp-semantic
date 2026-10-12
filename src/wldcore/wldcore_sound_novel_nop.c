#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 2: do nothing and fetch the next instruction. */
void wldcore_sound_novel_nop(void) {
    u16* flags;

    flags = &g_wldcore_sound_novel_record.state_flags;
    *flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
