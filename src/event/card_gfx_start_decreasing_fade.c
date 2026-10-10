#include "fft/event_card.h"
#include "psx/types.h"

/* Start the fade from intensity 0xf0 down to zero (the screen fades in). */
void card_gfx_start_decreasing_fade(void) {
    g_card_gfx_decreasing_fade_active = 1;
    g_card_gfx_fade_intensity = 0xf0;
}
