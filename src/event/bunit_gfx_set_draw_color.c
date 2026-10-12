#include "fft/event_bunit.h"

void bunit_gfx_set_draw_color(const rgb24_t* color) {
    if (color != 0) {
        g_bunit_gfx_sprite_color.r = color->r;
        g_bunit_gfx_sprite_color.g = color->g;
        g_bunit_gfx_sprite_color.b = color->b;
    }
}
