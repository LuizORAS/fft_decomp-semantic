#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound-novel opcode: fade the screen out over operand 0 frames. The
 * fade-out bit is cleared first so that wldcore_fade_start_screen always
 * starts the fade; the box-visible bit set here is replaced by it. */
void wldcore_opcode_fade_screen_out(void) {
    u32* state = (u32*)&g_wldcore_screen_fade_state;

    *state = (*state & ~WLDCORE_SCREEN_FADE_OUT) | WLDCORE_SCREEN_FADE_BOX_VISIBLE;
    wldcore_fade_start_screen(WLDCORE_SCREEN_FADE_OUT, g_wldcore_active_saved_record.instruction.bytes.operand_0);
    g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_FETCH;
}
