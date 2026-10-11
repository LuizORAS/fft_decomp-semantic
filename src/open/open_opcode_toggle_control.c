#include "fft/open.h"
#include "psx/types.h"

/* Opcode 0: end the script. It yields for the frame while the XA start or stop
 * sequence runs; after that it clears OPEN_SCRIPT_DISPATCH_RUNNING, which stops
 * the dispatcher. Two bytes. */
void open_opcode_toggle_control(void) {
    u32* control = &g_open_script_state.dispatch.flags;
    u32 state = *control;

    if (state & (OPEN_SCRIPT_DISPATCH_XA_STOPPING | OPEN_SCRIPT_DISPATCH_XA_STARTING)) {
        *control = state ^ OPEN_SCRIPT_DISPATCH_CONTINUE;
    } else {
        *control = state ^ OPEN_SCRIPT_DISPATCH_RUNNING;
        g_open_script_state.dispatch.byte_offset += 2;
    }
}
