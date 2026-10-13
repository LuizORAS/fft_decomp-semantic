#include "fft/world.h"
#include "psx/types.h"

/* Opcode 0x16: advance the window opening animation one step (capped at 4) and move past the
 * one-byte command. */
s32 world_menu_script_advance_window_scale_step(s32 script) {
    s8 step = g_world_menu_window_scale_step;

    if (step < 4) {
        g_world_menu_window_scale_step = step + 1;
    }
    return script + 1;
}
