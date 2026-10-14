#include "fft/world.h"
#include "psx/types.h"

/* Decode glyph `glyph` of the dialog font (10 x 14 pixels at 2 bits, 35 bytes) into the 4-bit image
 * `image` at `target`, adding `palette * 4` to every non-zero pixel, and return the glyph's advance width. */
s32 world_text_decode_font_glyph_to_4bpp(s32 glyph, u8* image, world_text_glyph_target_t* target, s32 palette) {
    s32 row;
    s32 col;
    u8* dst;
    u8* font;
    u8 pixel;
    u8 byte;

    font = g_world_text_glyph_bitmap_data;
    world_bit_cursor_set_primary(font + glyph * TEXT_GLYPH_BYTES);
    row = 0;
    if ((target->x & 1) == 0) {
        for (; row < 14; row++) {
            dst = image + (target->stride << 1) * (target->y + row) + (target->x >> 1);
            for (col = 0; col < 5; col++) {
                pixel = world_bits_read_primary(2);
                if (pixel != 0) {
                    pixel += palette * 4;
                }
                byte = pixel;
                pixel = world_bits_read_primary(2);
                if (pixel != 0) {
                    pixel += palette * 4;
                }
                byte |= pixel << 4;
                *dst++ = byte;
            }
        }
    } else {
        for (; row < 14; row++) {
            dst = image + (target->stride << 1) * (target->y + row) + (target->x >> 1);
            pixel = world_bits_read_primary(2);
            if (pixel != 0) {
                pixel += palette * 4;
            }
            byte = (*dst & 0xF) | (pixel << 4);
            *dst++ = byte;
            for (col = 0; col < 4; col++) {
                pixel = world_bits_read_primary(2);
                if (pixel != 0) {
                    pixel += palette * 4;
                }
                byte = pixel;
                pixel = world_bits_read_primary(2);
                if (pixel != 0) {
                    pixel += palette * 4;
                }
                byte |= pixel << 4;
                *dst++ = byte;
            }
            pixel = world_bits_read_primary(2);
            if (pixel != 0) {
                pixel += palette * 4;
            }
            *dst = pixel;
        }
    }
    return g_world_text_glyph_widths[glyph];
}
