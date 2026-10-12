#include "fft/wldcore.h"
#include "psx/types.h"

/* Copy an RGB triple, scaled by the screen brightness while a screen fade runs: each channel
 * becomes c * g_wldcore_screen_brightness_rgb / 128 (0x80 leaves it unchanged), clamped to 255.
 * Without a fade the triple is copied as is, which the 4-byte alignment-1 struct assignment emits as
 * the unaligned lwl/lwr + swl/swr quartet. */
void wldcore_gfx_copy_color_with_tint(CVECTOR* source, CVECTOR* out) {
    s32 red;
    s32 green;
    s32 blue;

    if (g_wldcore_screen_fade_state.flags[0] & WLDCORE_SCREEN_FADE_RUNNING) {
        red = ((source->r << 16) / 128 * g_wldcore_screen_brightness_rgb[0]) >> 16;
        green = ((source->g << 16) / 128 * g_wldcore_screen_brightness_rgb[1]) >> 16;
        blue = ((source->b << 16) / 128 * g_wldcore_screen_brightness_rgb[2]) >> 16;
        if (red >= 256) {
            red = 255;
        }
        if (green >= 256) {
            green = 255;
        }
        if (blue >= 256) {
            blue = 255;
        }
        out->r = red;
        out->g = green;
        out->b = blue;
    } else {
        *out = *source;
    }
}
