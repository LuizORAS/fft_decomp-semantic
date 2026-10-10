#include "fft/world.h"
#include "psx/types.h"

/* Fade the screen overlay from 0xff to 0 in steps of 4, one per frame. */
void world_gfx_ramp_screen_overlay_intensity_to_zero(void) {
    g_world_screen_fade = 0xff;
    do {
        world_thread_yield();
        g_world_screen_fade -= 4;
    } while (g_world_screen_fade > 0);
    g_world_screen_fade = 0;
}
