#include "fft/open.h"
#include "psx/types.h"

/* Opcode 14: yield each frame until the XA audio is neither playing, starting nor
 * stopping. Two bytes. */
void open_opcode_wait_for_xa_audio_idle(s16* unused_entry) {
    u32* control = &g_open_script_state.dispatch.flags;
    u32 state = *control;

    if (state
        & (OPEN_SCRIPT_DISPATCH_XA_PLAYING | OPEN_SCRIPT_DISPATCH_XA_STOPPING | OPEN_SCRIPT_DISPATCH_XA_STARTING)) {
        *control = state ^ OPEN_SCRIPT_DISPATCH_CONTINUE;
    } else {
        g_open_script_state.dispatch.cursor.byte_offset += 2;
    }
}
