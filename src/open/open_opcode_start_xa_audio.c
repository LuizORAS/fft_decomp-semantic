#include "fft/open.h"

void open_opcode_start_xa_audio(s16* command) {
    u32 flags = g_open_script_state.dispatch.flags;

    if (flags & 4) {
        if (flags & 0x20) {
            g_open_script_state.dispatch.flags = flags ^ 2;
            return;
        }
        g_open_script_state.dispatch.flags = flags ^ 4;
        g_open_script_state.dispatch.byte_offset += 4;
        return;
    }
    if (flags & 0x38) {
        g_open_script_state.dispatch.byte_offset += 4;
        return;
    }
    g_open_script_state.control_parameter = command[1];
    g_open_script_state.control_state = 0;
    g_open_script_state.dispatch.flags = (flags & ~0x3a) | 0x24;
    open_sound_set_type_and_volume(0, 1);
}
