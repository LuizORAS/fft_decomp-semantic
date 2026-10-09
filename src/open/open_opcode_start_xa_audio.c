#include "fft/open.h"

void open_opcode_start_xa_audio(s16* command) {
    u32 flags = g_open_script_state.dispatch.flags;

    if (flags & OPEN_SCRIPT_DISPATCH_WAITING) {
        if (flags & OPEN_SCRIPT_DISPATCH_XA_STARTING) {
            g_open_script_state.dispatch.flags = flags ^ OPEN_SCRIPT_DISPATCH_CONTINUE;
            return;
        }
        g_open_script_state.dispatch.flags = flags ^ OPEN_SCRIPT_DISPATCH_WAITING;
        g_open_script_state.dispatch.byte_offset += 4;
        return;
    }
    if (flags
        & (OPEN_SCRIPT_DISPATCH_XA_PLAYING | OPEN_SCRIPT_DISPATCH_XA_STOPPING | OPEN_SCRIPT_DISPATCH_XA_STARTING)) {
        g_open_script_state.dispatch.byte_offset += 4;
        return;
    }
    g_open_script_state.control_parameter = command[1];
    g_open_script_state.control_state = 0;
    g_open_script_state.dispatch.flags = (flags
                                             & ~(OPEN_SCRIPT_DISPATCH_CONTINUE | OPEN_SCRIPT_DISPATCH_XA_PLAYING
                                                 | OPEN_SCRIPT_DISPATCH_XA_STOPPING | OPEN_SCRIPT_DISPATCH_XA_STARTING))
        | (OPEN_SCRIPT_DISPATCH_WAITING | OPEN_SCRIPT_DISPATCH_XA_STARTING);
    open_sound_set_cd_mix_and_volume(0, 1);
}
