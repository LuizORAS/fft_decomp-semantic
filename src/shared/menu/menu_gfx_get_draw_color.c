#include "fft/menu.h"

/* Return the draw colour, the three RGB bytes that MENU_SCRIPT_OP_COLOURED_QUAD tints a quad with
 * (g_menu_gfx_sprite_color). */
rgb24_t* menu_gfx_get_draw_color(void) {
    return &g_menu_gfx_sprite_color;
}
