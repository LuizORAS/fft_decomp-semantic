#include "fft/event_equip.h"
#include "psx/types.h"

/* Stream command: advance the window transition frame by one, up to 4, and
 * skip the one-byte command. */
u8* equip_menu_script_advance_window_scale_step(u8* data) {
    s32 current;
    s16 previous;
    s32 unused[2];

    current = g_equip_gfx_transition_frame;
    previous = current;
    if (current < 4) {
        g_equip_gfx_transition_frame = previous + 1;
    }
    return data + 1;
}
