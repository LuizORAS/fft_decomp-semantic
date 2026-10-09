#include "fft/open.h"

void open_opcode_stop_xa_audio(void) {
    u32* control = &g_open_script_state.dispatch.flags;
    u32 state = *control;

    if (state & OPEN_SCRIPT_DISPATCH_WAITING) {
        if (state & OPEN_SCRIPT_DISPATCH_XA_STOPPING) {
            *control = state ^ OPEN_SCRIPT_DISPATCH_CONTINUE;
        } else {
            *control = state ^ OPEN_SCRIPT_DISPATCH_WAITING;
            g_open_script_state.dispatch.byte_offset += 2;
        }
    } else if (!(state & (OPEN_SCRIPT_DISPATCH_XA_PLAYING | OPEN_SCRIPT_DISPATCH_XA_STARTING))) {
        g_open_script_state.dispatch.byte_offset += 2;
    } else {
        g_open_script_state.control_state = 0;
        *control = (state
                       & ~(OPEN_SCRIPT_DISPATCH_CONTINUE | OPEN_SCRIPT_DISPATCH_XA_PLAYING
                           | OPEN_SCRIPT_DISPATCH_XA_STOPPING | OPEN_SCRIPT_DISPATCH_XA_STARTING))
            | (OPEN_SCRIPT_DISPATCH_WAITING | OPEN_SCRIPT_DISPATCH_XA_STOPPING);
        open_sound_set_cd_mix_and_volume(0, 1);
    }
}
