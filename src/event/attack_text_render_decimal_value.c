#include "fft/event_attack.h"
#include "psx/types.h"

void attack_text_render_decimal_value(s32 value, s32 flags, u8* pixels, world_glyph_blit_t* position) {
    s32 saved_color;
    s32 plus;
    s32 digits;
    s32 min_digits;
    s32 i;

    saved_color = position->color;
    position->color = 0;
    if ((flags & 0x0200) && value == 100) {
        flags = (flags & 0xff00) + 3;
    }
    if (flags & 0x8000) {
        position->rect.x -= 2;
        g_attack_text_decimal_glyph.source_x = 0xb4;
        blit_text_glyph(g_battle_menu_glyph_image, pixels, &g_attack_text_decimal_glyph, position);
        position->rect.x += 7;
    }
    if (flags & 0x0100) {
        g_attack_text_decimal_glyph.source_x = 0x78;
        g_attack_text_decimal_glyph.source_y = 0x1a;
        g_attack_text_decimal_glyph.height = 4;
        position->rect.y += 3;
        blit_text_glyph(g_battle_menu_glyph_image, pixels, &g_attack_text_decimal_glyph, position);
        position->rect.x += 7;
        position->rect.y -= 3;
        g_attack_text_decimal_glyph.source_y = 0x10;
        g_attack_text_decimal_glyph.height = 0x0a;
    }
    position->color = saved_color;
    if (flags & MENU_DECIMAL_SIGN_MINUS) {
        g_attack_text_decimal_glyph.source_x = 0xba;
        if (value == 0) {
            if ((flags & MENU_DECIMAL_FIELD_WIDTH_MASK) == 4) {
                position->rect.x += 10;
            } else {
                position->rect.x += 5;
            }
        }
        blit_text_glyph(g_battle_menu_glyph_image, pixels, &g_attack_text_decimal_glyph, position);
        position->rect.x += 5;
        if (value == 0) {
            return;
        }
    }
    plus = flags & MENU_DECIMAL_SIGN_PLUS;
    if (plus) {
        g_attack_text_decimal_glyph.source_x = 0xc8;
        blit_text_glyph(g_battle_menu_glyph_image, pixels, &g_attack_text_decimal_glyph, position);
        position->rect.x += 5;
    }
    digits = flags & MENU_DECIMAL_FIELD_WIDTH_MASK;
    if (flags & 0x2000) {
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
        g_attack_text_decimal_glyph.source_x = value % 10 * 6 + 0x78;
        blit_text_glyph(g_battle_menu_glyph_image, pixels, &g_attack_text_decimal_glyph, position);
        position->rect.x -= 5;
        value /= 10;
        if (!(flags & 0x4000) && value == 0) {
            break;
        }
    }
    position->color = 0;
}
