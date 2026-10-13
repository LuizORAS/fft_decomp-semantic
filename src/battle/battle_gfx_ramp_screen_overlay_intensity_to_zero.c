#include "fft/battle_gfx.h"
#include "fft/battle_thread.h"
#include "fft/thread.h"

/* Fades the screen back in: lowers g_battle_screen_fade from 255 by 4 per frame,
 * yielding once per step, and ends on 0. */
void battle_gfx_ramp_screen_overlay_intensity_to_zero(void) {
    g_battle_screen_fade = 0xFF;
    do {
        thread_yield();
        g_battle_screen_fade -= 4;
    } while (g_battle_screen_fade > 0);
    g_battle_screen_fade = 0;
}
