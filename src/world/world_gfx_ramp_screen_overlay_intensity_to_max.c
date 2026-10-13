#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* The screen-covering tile consumes g_world_screen_fade as RGB intensity, not alpha.
 * Yield once per step, then clamp to 255. */
void world_gfx_ramp_screen_overlay_intensity_to_max(void) {
    g_world_screen_fade = 0;
    do {
        thread_yield();
        g_world_screen_fade += 4;
    } while (g_world_screen_fade < 256);
    g_world_screen_fade = 255;
}
