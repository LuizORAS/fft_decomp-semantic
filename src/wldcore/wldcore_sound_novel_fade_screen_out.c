#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 40: fade the screen out over operand 0 frames. The fade-out bit is cleared
 * first so that wldcore_gfx_start_screen_fade always starts the fade; the box-visible bit set here is
 * replaced by it. */
void wldcore_sound_novel_fade_screen_out(void) {
    g_wldcore_screen_fade_state.flags[0]
        = (g_wldcore_screen_fade_state.flags[0] & ~WLDCORE_SCREEN_FADE_OUT) | WLDCORE_SCREEN_FADE_BOX_VISIBLE;
    wldcore_gfx_start_screen_fade(WLDCORE_SCREEN_FADE_OUT, g_wldcore_sound_novel_record.instruction.bytes.operand_0);
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
