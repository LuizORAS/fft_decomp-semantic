#include "fft/battle.h"
#include "psx/types.h"

/* Draw `value` in the dialog font's digit glyphs (glyphs 0-9, 35-byte records) into the text image `image`,
 * right-aligned in `digit_count` 6-pixel cells from the menu text origin and not zero-padded, in
 * g_battle_menu_text_color, which it then resets to 0.
 *
 * The cell step goes through a local: `origin->rect.x -= 6` on the halfword
 * lvalue is `+ 65530` in HImode with its own constant insn, which makes the
 * loop 27 RTL insns, and `loop.c:move_movables` then leaves the div-by-10 magic
 * constant in the loop body; the target hoists it into $s2. */
void battle_menu_draw_font_digits(s32 value, u8 digit_count, s32 image, world_glyph_blit_t* origin) {
    s16 cell;

    origin->rect.x += (digit_count - 1) * 6;
    do {
        battle_text_render_glyph_to_4bpp_image(
            g_battle_text_glyph_bitmap_data + (value % 10) * TEXT_GLYPH_BYTES, image, origin, g_battle_menu_text_color);
        value = value / 10;
        cell = origin->rect.x;
        origin->rect.x = cell - 6;
    } while (value != 0);
    g_battle_menu_text_color = 0;
}
