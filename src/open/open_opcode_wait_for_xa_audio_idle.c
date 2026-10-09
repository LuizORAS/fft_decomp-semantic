#include "fft/open.h"
#include "psx/types.h"

void open_opcode_wait_for_xa_audio_idle(void) {
    u32* control = &g_open_script_state.dispatch.flags;
    u32 state = *control;

    if (state
        & (OPEN_SCRIPT_DISPATCH_XA_PLAYING | OPEN_SCRIPT_DISPATCH_XA_STOPPING | OPEN_SCRIPT_DISPATCH_XA_STARTING)) {
        *control = state ^ OPEN_SCRIPT_DISPATCH_CONTINUE;
    } else {
        g_open_script_state.dispatch.byte_offset += 2;
    }
}
