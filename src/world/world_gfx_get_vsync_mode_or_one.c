#include "fft/world.h"

/* Return g_world_gfx_vsync_mode, or 1 while it is 0: the step WORLD's fades, cursors and input
 * repeats advance by each frame. */
s32 world_gfx_get_vsync_mode_or_one(void) {
    s32 value = g_world_gfx_vsync_mode;
    if (value == 0) {
        return 1;
    }
    return value;
}
