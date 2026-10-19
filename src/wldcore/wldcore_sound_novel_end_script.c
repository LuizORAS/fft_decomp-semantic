#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 0: end the reading (WLDCORE_SOUND_NOVEL_STATE_END);
 * wldcore_sound_novel_run_until_yield then returns 0. */
void wldcore_sound_novel_end_script(void) {
    u16* flags;

    flags = &g_wldcore_sound_novel_record.state_flags;
    *flags |= WLDCORE_SOUND_NOVEL_STATE_END;
}
