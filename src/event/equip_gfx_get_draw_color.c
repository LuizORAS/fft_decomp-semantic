#include "fft/event_equip.h"
#include "psx/types.h"

/* Return the draw colour, the three RGB bytes that MENU_SCRIPT_OP_COLOURED_QUAD tints a quad with
 * (g_equip_gfx_sprite_color). */
u8* equip_gfx_get_draw_color(void) {
    return g_equip_gfx_sprite_color;
}
