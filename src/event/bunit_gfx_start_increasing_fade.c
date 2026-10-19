#include "fft/event_bunit.h"

/* Start the fade from intensity 0 up to full (the screen fades out). */
void bunit_gfx_start_increasing_fade(void) {
    g_bunit_gfx_increasing_fade_active = 1;
    g_bunit_gfx_fade_intensity = 0;
}
