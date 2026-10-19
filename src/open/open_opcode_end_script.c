#include "fft/open.h"
#include "psx/types.h"

/* Opcode 0: end the script (clear OPEN_SCRIPT_DISPATCH_RUNNING and step past the opcode). While an
 * XA start or stop sequence still runs it waits instead, toggling OPEN_SCRIPT_DISPATCH_CONTINUE to end
 * the frame. The flags are toggled, not set or cleared. */
void open_opcode_end_script(s16* unused_entry) {
    u32 state = g_open_script_state.dispatch.flags;

    if (state & (OPEN_SCRIPT_DISPATCH_XA_STOPPING | OPEN_SCRIPT_DISPATCH_XA_STARTING)) {
        g_open_script_state.dispatch.flags = state ^ OPEN_SCRIPT_DISPATCH_CONTINUE;
    } else {
        g_open_script_state.dispatch.flags = state ^ OPEN_SCRIPT_DISPATCH_RUNNING;
        g_open_script_state.dispatch.cursor.byte_offset += 2;
    }
}
