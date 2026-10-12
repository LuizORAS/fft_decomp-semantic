#include "fft/world.h"
#include "psx/types.h"

/* Blit the two halves of the tall shadow glyph, one above the other, 2
 * pixels left of and below the pen position. */
void world_menu_draw_stacked_glyph_pair(void* buffer, world_glyph_blit_t* dest) {
    dest->rect.x -= 2;
    dest->rect.y += 2;
    world_text_blit_glyph(g_world_menu_glyph_image, buffer, g_world_menu_stacked_glyph_upper, dest);

    dest->rect.y += 4;
    world_text_blit_glyph(g_world_menu_glyph_image, buffer, g_world_menu_stacked_glyph_lower, dest);
}
