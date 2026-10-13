#include "fft/event_bunit.h"
#include "psx/types.h"

/* Return the draw colour, the three RGB bytes that MENU_SCRIPT_OP_COLOURED_QUAD tints a quad with
 * (g_bunit_gfx_sprite_color). */
u8* bunit_gfx_get_draw_color(void) {
    return g_bunit_gfx_sprite_color;
}
