#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 1: end the section and yield; the reading goes on with the next section (the
 * record's section, which entering a section leaves one past it). */
void wldcore_sound_novel_end_section(void) {
    u16* flags;

    flags = &g_wldcore_sound_novel_record.state_flags;
    *flags |= WLDCORE_SOUND_NOVEL_STATE_BRANCH | WLDCORE_SOUND_NOVEL_STATE_YIELD;
}
