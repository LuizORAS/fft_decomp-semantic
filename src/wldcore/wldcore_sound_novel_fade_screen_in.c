#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 41: fade the screen in over operand 0 frames and drop the white-overlay state. */
void wldcore_sound_novel_fade_screen_in(void) {
    u32* state = &g_wldcore_screen_fade_state.flags[0];
    u16* flags = &g_wldcore_sound_novel_record.state_flags;

    *state |= WLDCORE_SCREEN_FADE_OUT;
    *flags &= ~WLDCORE_SOUND_NOVEL_STATE_WHITE_OVERLAY;
    wldcore_gfx_start_screen_fade(0, g_wldcore_sound_novel_record.instruction.bytes.operand_0);
    *flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
