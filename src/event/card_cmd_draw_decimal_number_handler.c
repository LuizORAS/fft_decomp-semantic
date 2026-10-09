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
    u16 w;
    u16 h;
    RECT _unused_10;
} card_number_glyph_t;

/* Render command that draws command[9] as command[4] zero-padded digit tiles,
 * command[5] pixels apart, starting at (command[7], command[8]). Styles 2 and
 * up (command[2]) draw nothing. */
u8* card_cmd_draw_decimal_number_handler(u8* command) {
    s8 text[16];
    card_number_glyph_t glyph;
    s32 y;
    s16 pad;
    s32 pos;
    s32 value;
    s32 digit;
    s32 i;
    u16 clut;
    const u8* color;

    value = command[9];
    y = command[8];
    clut = g_card_text_digit_clut;
    glyph.u = 0x78;
    glyph.v = 0x10;
    glyph.w = 6;
    glyph.h = 0xB;
    pad = command[4] - sprintf((char*)text, g_card_text_decimal_format, value);
    pos = 0;
    color = g_card_text_color;
    for (i = 0; i < command[4]; i++) {
        glyph.rect.x = command[7] + (i * command[5]);
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
        if (command[2] < 2) {
            card_gfx_enqueue_textured_quad(&glyph.rect, glyph.u + ((s16)glyph.w * digit), glyph.v, color,
                g_card_gfx_draw_semitrans, g_card_text_digit_texture_page, clut, g_card_text_otag_index);
        }
    }
    return command + command[1];
}
