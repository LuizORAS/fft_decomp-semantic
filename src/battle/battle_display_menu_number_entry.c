#include "fft/battle.h"
#include "psx/types.h"

/* Draw a decimal value right-to-left from the menu text origin.
 *
 * The origin is first advanced to the last of `digit_count` cells and then
 * walked back one 6-pixel cell per digit, so a shorter value leaves the leading
 * cells untouched rather than zero-padding them. Digits index the same 35-byte
 * glyph records the menu text renderers use.
 *
 * The cell step goes through a local: `origin->rect.x -= 6` on the halfword
 * lvalue is `+ 65530` in HImode with its own constant insn, which makes the
 * loop 27 RTL insns, and `loop.c:move_movables` then leaves the div-by-10 magic
 * constant in the loop body; the target hoists it into $s2. */
void battle_display_menu_number_entry(s32 value, u8 digit_count, s32 image, world_glyph_blit_t* origin) {
    s16 cell;

    origin->rect.x += (digit_count - 1) * 6;
    do {
        battle_text_render_glyph_to_4bpp_image(
            g_text_glyph_bitmap_data + (value % 10) * 35, image, origin, g_battle_menu_text_palette_offset);
        value = value / 10;
        cell = origin->rect.x;
        origin->rect.x = cell - 6;
    } while (value != 0);
    g_battle_menu_text_palette_offset = 0;
}
