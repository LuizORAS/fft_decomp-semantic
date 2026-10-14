#include "fft/world.h"
#include "psx/types.h"

/* Set the source rectangle of the glyph blit (g_world_text_glyph_source), for the menu-screen window
 * frame threads. */
void world_gfx_set_blit_source_rect(s16 x, s16 y, s16 width, s16 height) {
    g_world_text_glyph_source.rect.x = x;
    g_world_text_glyph_source.rect.y = y;
    g_world_text_glyph_source.rect.w = width;
    g_world_text_glyph_source.rect.h = height;
}
