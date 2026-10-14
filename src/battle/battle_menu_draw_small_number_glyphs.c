#include "fft/battle.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Draw `value` right to left in the menu glyph sheet's small digits (10 rows, 5 pixels apart) as
 * `digits & 0xff` glyphs at `pos`, with the sign, slash, dot and suffix glyphs the flag bits of `digits`
 * select. Modes 0xC00 and 0xE00 draw three placeholder glyphs instead, and while
 * g_battle_menu_hide_numeric_values is set every digit is the placeholder. The unit panels' figures use it;
 * the large digits are battle_menu_draw_large_number_glyphs. */
void battle_menu_draw_small_number_glyphs(s32 value, s32 digits, void* resource, world_glyph_blit_t* pos) {
    s32 i;
    s32 cursor;
    RECT* src;

    src = &g_battle_menu_small_number_glyph_rect;
    cursor = digits & 0xFF00;
    if (cursor == 0xC00 || cursor == 0xE00) {
        g_battle_menu_small_number_glyph_rect.y = 0x10;
        if (digits & 1) {
            src->x = 0xB4;
            battle_text_blit_glyph(g_battle_menu_glyph_image, resource, src, pos);
            pos->rect.x += 5;
        }
        src->x = 0xBA;
        if (cursor == 0xE00) {
            src->x = 0xD0;
        }
        for (i = 0; i < 3; i++) {
            battle_text_blit_glyph(g_battle_menu_glyph_image, resource, &g_battle_menu_small_number_glyph_rect, pos);
            pos->rect.x += 5;
        }
        return;
    }
    if (digits & 0x800) {
        src->x = 0xBA;
        battle_text_blit_glyph(g_battle_menu_glyph_image, resource, src, pos);
        pos->rect.x += 7;
    }
    if (g_battle_menu_hide_numeric_values == 0) {
        if ((digits & 0x200) && value == 0x64) {
            digits = cursor + 3;
        }
        if ((digits & 0x400) && value != 0x64) {
            digits = (digits & 0xFF00) + 2;
            pos->rect.x += 6;
        }
    }
    if (digits & 0x1000) {
        src->x = 0xB4;
        battle_text_blit_glyph(g_battle_menu_glyph_image, resource, &g_battle_menu_small_number_glyph_rect, pos);
        pos->rect.x += 5;
    }
    if (digits & 0x8000) {
        pos->rect.x -= 2;
        src->x = 0xB4;
        battle_text_blit_glyph(g_battle_menu_glyph_image, resource, &g_battle_menu_small_number_glyph_rect, pos);
        pos->rect.x += 7;
    }
    if (digits & 0x100) {
        src->x = 0x78;
        src->y = 0x1A;
        src->h = 4;
        pos->rect.y += 3;
        battle_text_blit_glyph(g_battle_menu_glyph_image, resource, &g_battle_menu_small_number_glyph_rect, pos);
        pos->rect.x += 7;
        pos->rect.y -= 3;
        src->y = 0x10;
        src->h = 0xA;
    }
    pos->rect.x += ((digits & 0xFF) - 1) * 5;
    cursor = pos->rect.x + 5;
    for (i = 0; i < (digits & 0xFF); i++) {
        src->x = (value % 10) * 6 + 0x78;
        if (g_battle_menu_hide_numeric_values != 0) {
            src->x = 0xD0;
        }
        battle_text_blit_glyph(g_battle_menu_glyph_image, resource, &g_battle_menu_small_number_glyph_rect, pos);
        value /= 10;
        pos->rect.x -= 5;
    }
    pos->rect.x = cursor;
    if (digits & 0x2000) {
        src->x = 0xC0;
        battle_text_blit_glyph(g_battle_menu_glyph_image, resource, &g_battle_menu_small_number_glyph_rect, pos);
        pos->rect.x += 5;
    }
}
