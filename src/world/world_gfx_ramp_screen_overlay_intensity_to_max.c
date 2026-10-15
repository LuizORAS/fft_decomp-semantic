#include "fft/world.h"
#include "psx/types.h"

/* Fade the screen out: raise g_world_screen_fade from 0 by 4 per frame, yielding once per step, and
 * end on 255. The screen-covering tile takes the value as an RGB intensity, not as alpha. */
void world_gfx_ramp_screen_overlay_intensity_to_max(void) {
    s32 level;

    g_world_screen_fade = 0;
    do {
        world_thread_yield();
        level = g_world_screen_fade + 4;
        g_world_screen_fade = level;
    } while (level < 256);
    g_world_screen_fade = 255;
}
