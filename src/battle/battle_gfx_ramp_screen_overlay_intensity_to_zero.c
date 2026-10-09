#include "fft/battle.h"
#include "psx/types.h"

void battle_gfx_ramp_screen_overlay_intensity_to_zero(void) {
    g_battle_screen_fade = 0xFF;
    do {
        battle_thread_yield();
        g_battle_screen_fade -= 4;
    } while (g_battle_screen_fade > 0);
    g_battle_screen_fade = 0;
}
