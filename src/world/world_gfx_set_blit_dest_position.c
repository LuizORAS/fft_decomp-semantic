#include "fft/world.h"
#include "psx/types.h"

/* Set where the glyph blit draws (g_world_text_glyph_dest), for the menu-screen window frame
 * threads. */
void world_gfx_set_blit_dest_position(s32 x, s32 y) {
    g_world_text_glyph_dest.rect.x = x;
    g_world_text_glyph_dest.rect.y = y;
}
