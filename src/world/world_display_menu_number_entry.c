#include "fft/world.h"
#include "psx/types.h"

/* Draw a decimal value right-to-left from the menu text origin.
 *
 * The origin starts at the last requested digit cell and moves back 6 pixels
 * per emitted digit, so shorter values are not zero-padded. Reading the cell
 * position unsigned through an `s32` temporary preserves the target's hoisted
 * divide-by-10 constant. */
void world_display_menu_number_entry(s32 value, u8 digit_count, u8* image, glyph_blit_t* origin) {
    origin->rect.x += (digit_count - 1) * 6;
    do {
        world_text_blit_font_glyph_to_4bpp(
            g_world_text_glyph_bitmap_data + (value % 10) * TEXT_GLYPH_BYTES, image, origin, g_world_menu_text_color);
        value = value / 10;
        origin->rect.x -= 6;
    } while (value != 0);
    g_world_menu_text_color = 0;
}
