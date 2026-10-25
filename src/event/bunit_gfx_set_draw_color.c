#include "fft/event_bunit.h"
#include "fft/menu.h"

/* Copy the three RGB bytes at `color` into the draw color (g_menu_gfx_sprite_color); a null `color` changes nothing.
 */
void bunit_gfx_set_draw_color(const rgb24_t* color) {
    if (color != 0) {
        g_menu_gfx_sprite_color.r = color->r;
        g_menu_gfx_sprite_color.g = color->g;
        g_menu_gfx_sprite_color.b = color->b;
    }
}
