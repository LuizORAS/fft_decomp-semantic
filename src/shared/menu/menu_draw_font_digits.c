#include "fft/battle_text.h"
#include "fft/menu.h"

/* Draw `value` in the dialog font's digit glyphs (glyphs 0-9, 35-byte records) into the text image `image`,
 * right-aligned in `digit_count` 6-pixel cells from the menu text origin and not zero-padded, in
 * g_menu_text_palette_offset, which it then resets to 0. WORLD compiles this text too
 * (world_menu_draw_font_digits).
 *
 * The cell step goes through a local: `origin->rect.x -= 6` on the 16-bit value
 * lvalue is `+ 65530` in HImode with its own constant insn, which makes the
 * loop 27 RTL insns, and `loop.c:move_movables` then leaves the div-by-10 magic
 * constant in the loop body; the target hoists it into $s2. */
void menu_draw_font_digits(s32 value, u8 digit_count, u8* image, world_glyph_blit_t* origin) {
    s16 cell;

    origin->rect.x += (digit_count - 1) * 6;
    do {
        battle_text_render_glyph_to_4bpp_image(
            g_text_glyph_bitmap_data + (value % 10) * TEXT_GLYPH_BYTES, image, origin, g_menu_text_palette_offset);
        value = value / 10;
        cell = origin->rect.x;
        origin->rect.x = cell - 6;
    } while (value != 0);
    g_menu_text_palette_offset = 0;
}
