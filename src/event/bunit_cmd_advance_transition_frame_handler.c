#include "fft/event_bunit.h"
#include "psx/types.h"

/* Command 0x16: step the portrait transition, then skip the one-byte command. */
u8* bunit_cmd_advance_transition_frame_handler(u8* data) {
    if (g_bunit_gfx_transition_frame < 4) {
        g_bunit_gfx_transition_frame++;
    }
    return data + 1;
}
