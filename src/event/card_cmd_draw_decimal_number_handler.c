#include "fft/event_card.h"
#include "psx/libc.h"
#include "psx/types.h"

/* Destination rectangle followed by the source glyph cell. The frame also
 * holds the eight bytes where EQUIP's twin keeps its minus-sign rectangle;
 * CARD never touches them. */
typedef struct card_number_glyph {
    RECT rect;
    s16 u;
    s16 v;
    s16 w;
    u16 h;
    RECT _unused_10;
} card_number_glyph_t;

/* Draw the literal value as zero-padded digits at the requested position. */
menu_draw_number_command_t* card_cmd_draw_decimal_number_handler(menu_draw_number_command_t* command) {
    s8 text[16];
    card_number_glyph_t glyph;
    s32 y;
    s16 pad;
    s32 pos;
    s32 value;
    s32 digit;
    s32 i;
    u16 clut;
    const rgb24_t* color;

    value = command->value;
    y = command->y;
    clut = g_card_menu_alternate_number_clut;
    glyph.u = 0x78;
    glyph.v = 0x10;
    glyph.w = 6;
    glyph.h = 0xB;
    pad = command->width - sprintf(text, g_card_text_decimal_format, value);
    pos = 0;
    color = &g_card_text_color;
    for (i = 0; i < command->width; i++) {
        glyph.rect.x = command->x + (i * command->spacing);
        glyph.rect.y = y;
        glyph.rect.w = glyph.w;
        glyph.rect.h = glyph.h;
        if (pad != 0) {
            digit = 0;
            pad -= 1;
        } else {
            digit = text[pos] - '0';
            pos += 1;
        }
        if (command->style < 2) {
            card_gfx_enqueue_textured_quad(&glyph.rect, glyph.u + (glyph.w * digit), glyph.v, color,
                g_card_gfx_draw_semitrans, g_card_text_digit_texture_page, clut, g_card_text_otag_index);
        }
    }
    return (menu_draw_number_command_t*)((u8*)command + command->length);
}
