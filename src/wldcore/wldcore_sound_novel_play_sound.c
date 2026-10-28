#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 31: play sound effect operand 0 on a free channel. */
void wldcore_sound_novel_play_sound(void) {
    main_sound_play_sfx_find_channel(g_wldcore_sound_novel_record.instruction.bytes.operand_0);
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
