#include "fft/world.h"
#include "psx/types.h"

/* Render a decimal value with optional sign/plus glyphs into a text image. */
void world_text_render_decimal_value(s32 value, s32 flags, void* pixels, world_glyph_blit_t* position) {
    s32 saved_color;
    s32 plus;
    s32 digits;
    s32 min_digits;
    s32 i;

    saved_color = position->color;
    position->color = 0;
    if ((flags & MENU_DECIMAL_WIDEN_FOR_100) && value == 100) {
        flags = (flags & 0xff00) + 3;
    }
    if (flags & MENU_DECIMAL_SLASH_PREFIX) {
        position->rect.x -= 2;
        g_world_decimal_glyph_source.rect.x = 0xb4;
        world_text_blit_glyph(g_world_menu_glyph_sheet, pixels, &g_world_decimal_glyph_source, position);
        position->rect.x += 7;
    }
    if (flags & MENU_DECIMAL_DOTS_PREFIX) {
        g_world_decimal_glyph_source.rect.x = 0x78;
        g_world_decimal_glyph_source.rect.y = 0x1a;
        g_world_decimal_glyph_source.rect.h = 4;
        position->rect.y += 3;
        world_text_blit_glyph(g_world_menu_glyph_sheet, pixels, &g_world_decimal_glyph_source, position);
        position->rect.x += 7;
        position->rect.y -= 3;
        g_world_decimal_glyph_source.rect.y = 0x10;
        g_world_decimal_glyph_source.rect.h = 0x0a;
    }
    position->color = saved_color;
    if (flags & MENU_DECIMAL_SIGN_MINUS) {
        g_world_decimal_glyph_source.rect.x = 0xba;
        if (value == 0) {
            if ((flags & MENU_DECIMAL_FIELD_WIDTH_MASK) == 4) {
                position->rect.x += 10;
            } else {
                position->rect.x += 5;
            }
        }
        world_text_blit_glyph(g_world_menu_glyph_sheet, pixels, &g_world_decimal_glyph_source, position);
        position->rect.x += 5;
        if (value == 0) {
            return;
        }
    }
    plus = flags & MENU_DECIMAL_SIGN_PLUS;
    if (plus) {
        g_world_decimal_glyph_source.rect.x = 0xc8;
        world_text_blit_glyph(g_world_menu_glyph_sheet, pixels, &g_world_decimal_glyph_source, position);
        position->rect.x += 5;
    }
    digits = flags & MENU_DECIMAL_FIELD_WIDTH_MASK;
    if (flags & MENU_DECIMAL_THREE_COLUMN_FIELD) {
        digits = 3;
    }
    if (plus) {
        digits--;
    }
    if (flags & MENU_DECIMAL_SIGN_MINUS) {
        digits--;
    }
    /* Retail gives every rung the same width, so the value is always placed as one digit. */
    if (value < 10) {
        min_digits = 1;
    } else if (value < 100) {
        min_digits = 1;
    } else {
        min_digits = 1;
    }
    if ((flags & MENU_DECIMAL_FIELD_WIDTH_MASK) != 1) {
        position->rect.x += (digits - min_digits) * 5;
    }
    for (i = 0; i < (flags & 0xff); i++) {
        g_world_decimal_glyph_source.rect.x = value % 10 * 6 + 0x78;
        world_text_blit_glyph(g_world_menu_glyph_sheet, pixels, &g_world_decimal_glyph_source, position);
        position->rect.x -= 5;
        value /= 10;
        if (!(flags & MENU_DECIMAL_LEADING_ZEROS) && value == 0) {
            break;
        }
    }
    position->color = 0;
}
