#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound-novel opcode: fade the screen in over operand 0 frames and drop the
 * white-overlay state. */
void wldcore_opcode_fade_screen_in(void) {
    u32* state = (u32*)&g_wldcore_screen_fade_state;
    u16* flags = &g_wldcore_active_saved_record.state_flags;

    *state |= WLDCORE_SCREEN_FADE_OUT;
    *flags &= ~WLDCORE_OPCODE_STATE_WHITE_OVERLAY;
    wldcore_fade_start_screen(0, (u8)(g_wldcore_active_saved_record.instruction >> 8));
    *flags |= WLDCORE_OPCODE_STATE_FETCH;
}
