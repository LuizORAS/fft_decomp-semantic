#include "fft/wldcore.h"
#include "psx/types.h"

void wldcore_opcode_fade_screen_out(void) {
    u32* state = (u32*)&g_wldcore_screen_fade_state;

    *state = (*state & ~WLDCORE_SCREEN_FADE_OUT) | WLDCORE_SCREEN_FADE_BOX_VISIBLE;
    wldcore_fade_start_screen(WLDCORE_SCREEN_FADE_OUT, g_wldcore_active_saved_record.instruction.bytes.operand_0);
    g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_FETCH;
}
