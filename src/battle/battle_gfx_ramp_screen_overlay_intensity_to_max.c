#include "fft/battle_gfx.h"
#include "fft/battle_thread.h"

/* Fades the screen out: raises g_battle_screen_fade from 0 by 4 per frame,
 * yielding once per step, and ends on 255. The screen-covering tile uses the
 * value as a grey level to subtract. */
void battle_gfx_ramp_screen_overlay_intensity_to_max(void) {
    g_battle_screen_fade = 0;
    do {
        battle_thread_yield();
        g_battle_screen_fade += 4;
    } while (g_battle_screen_fade < 0x100);
    g_battle_screen_fade = 0xff;
}
