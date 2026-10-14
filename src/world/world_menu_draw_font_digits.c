#include "fft/world.h"
#include "psx/types.h"

/* Same as battle_menu_draw_font_digits: draw `value` in the dialog font's digit glyphs into a text image,
 * right-aligned in `digit_count` 6-pixel cells from the menu text origin, then reset the text colour. */
void world_menu_draw_font_digits(s32 value, u8 digit_count, void* image, world_glyph_blit_t* origin) {
    origin->rect.x += (digit_count - 1) * 6;
    do {
        world_text_blit_font_glyph_to_4bpp(
            g_world_text_glyph_bitmap_data + (value % 10) * TEXT_GLYPH_BYTES, image, origin, g_world_menu_text_color);
        value = value / 10;
        origin->rect.x -= 6;
    } while (value != 0);
    g_world_menu_text_color = 0;
}
