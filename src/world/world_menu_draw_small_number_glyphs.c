#include "fft/world.h"
#include "psx/types.h"

/* Same as battle_menu_draw_small_number_glyphs: draw `value` right to left in the menu glyph sheet's
 * small digits (10 rows, 5 pixels apart) as `flags & 0xff` glyphs, with the sign and prefix glyphs the high
 * flag bits select. Modes 0xC00 and 0xE00 draw three placeholder glyphs (0xBA or 0xD0) instead, optionally
 * after the 0xB4 glyph. The cursor ends five pixels past the last digit.
 *
 * `work` holds the prefix mode and later the final cursor x; sharing one variable reproduces the target's
 * register assignment. */
void world_menu_draw_small_number_glyphs(s32 value, s32 flags, void* pixels, world_glyph_blit_t* position) {
    world_glyph_blit_t* glyph;
    s32 work;
    s32 i;

    work = flags & 0xFF00;
    glyph = &g_world_menu_small_number_glyph_rect;
    if (work == 0xC00 || work == 0xE00) {
        g_world_menu_small_number_glyph_rect.rect.y = 0x10;
        if (flags & 1) {
            glyph->rect.x = 0xB4;
            world_text_blit_glyph(g_world_menu_glyph_image, pixels, glyph, position);
            position->rect.x += 5;
        }
        glyph->rect.x = 0xBA;
        if (work == 0xE00) {
            glyph->rect.x = 0xD0;
        }
        for (i = 0; i < 3; i++) {
            world_text_blit_glyph(g_world_menu_glyph_image, pixels, &g_world_menu_small_number_glyph_rect, position);
            position->rect.x += 5;
        }
        return;
    }
    if (g_world_menu_hide_numeric_values == 0) {
        if ((flags & 0x200) && value == 100) {
            flags = work + 3;
        }
        if ((flags & 0x400) && value != 100) {
            flags = (flags & 0xFF00) + 2;
            position->rect.x += 6;
        }
    }
    if (flags & 0x1000) {
        glyph->rect.x = 0xB4;
        world_text_blit_glyph(g_world_menu_glyph_image, pixels, &g_world_menu_small_number_glyph_rect, position);
        position->rect.x += 5;
    }
    if (flags & 0x8000) {
        position->rect.x -= 2;
        glyph->rect.x = 0xB4;
        world_text_blit_glyph(g_world_menu_glyph_image, pixels, &g_world_menu_small_number_glyph_rect, position);
        position->rect.x += 7;
    }
    if (flags & 0x100) {
        glyph->rect.x = 0x78;
        glyph->rect.y = 0x1A;
        glyph->rect.h = 4;
        position->rect.y += 3;
        world_text_blit_glyph(g_world_menu_glyph_image, pixels, &g_world_menu_small_number_glyph_rect, position);
        position->rect.x += 7;
        position->rect.y -= 3;
        glyph->rect.y = 0x10;
        glyph->rect.h = 0xA;
    }
    position->rect.x += ((flags & 0xFF) - 1) * 5;
    work = position->rect.x + 5;
    for (i = 0; i < (flags & 0xFF); i++) {
        glyph->rect.x = value % 10 * 6 + 0x78;
        if (flags & 0x800) {
            glyph->rect.x = 0xBA;
        }
        world_text_blit_glyph(g_world_menu_glyph_image, pixels, &g_world_menu_small_number_glyph_rect, position);
        value /= 10;
        position->rect.x -= 5;
    }
    position->rect.x = work;
    if (flags & 0x2000) {
        glyph->rect.x = 0xC0;
        world_text_blit_glyph(g_world_menu_glyph_image, pixels, &g_world_menu_small_number_glyph_rect, position);
        position->rect.x += 5;
    }
}
