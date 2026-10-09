#include "fft/event_bunit.h"
#include "psx/types.h"

/* Fill a background rectangle with 16-pixel textured tiles.
 *
 * Command bytes 5 and 6 encode the full-tile counts in their high nibbles and
 * the remaining width and height in their low nibbles.
 *
 * This is the BUNIT.OUT twin of jobstts_cmd_draw_background_tiles_handler.
 */
u8* bunit_cmd_draw_background_tiles_handler(u8* desc) {
    RECT rect;
    s32 full_columns;
    s32 origin_x;
    s32 origin_y;
    s32 full_rows;
    s32 partial_width;
    s32 partial_height;
    s32 otag_index;
    u16 clut_id;
    u32 packed_height;
    u32 packed_width;
    s32 row;
    s32 column;

    packed_height = desc[6];
    packed_width = desc[5];
    otag_index = g_bunit_gfx_otag_index;
    clut_id = g_bunit_text_metric_2;
    full_columns = packed_width >> 4;
    partial_width = packed_width & 0xF;
    full_rows = packed_height >> 4;
    partial_height = packed_height & 0xF;
    origin_x = desc[3];
    origin_y = desc[4];

    for (row = 0; row < full_rows; row++) {
        rect.x = origin_x;
        rect.y = origin_y + (row * 16);
        rect.w = partial_width;
        rect.h = 16;
        bunit_gfx_enqueue_textured_quad(
            &rect, 8, 8, 0, g_bunit_gfx_semitrans_enabled, g_bunit_text_digit_texture_page, clut_id, otag_index);
        for (column = 0; column < full_columns; column++) {
            rect.x = origin_x + partial_width + (column * 16);
            rect.y = origin_y + (row * 16);
            rect.w = 16;
            rect.h = 16;
            bunit_gfx_enqueue_textured_quad(
                &rect, 8, 8, 0, g_bunit_gfx_semitrans_enabled, g_bunit_text_digit_texture_page, clut_id, otag_index);
        }
    }

    if (partial_height != 0) {
        rect.x = origin_x;
        rect.y = origin_y + (row * 16);
        rect.w = partial_width;
        rect.h = partial_height;
        bunit_gfx_enqueue_textured_quad(
            &rect, 8, 8, 0, g_bunit_gfx_semitrans_enabled, g_bunit_text_digit_texture_page, clut_id, otag_index);
        for (column = 0; column < full_columns; column++) {
            rect.x = origin_x + partial_width + (column * 16);
            rect.y = origin_y + (row * 16);
            rect.w = 16;
            rect.h = partial_height;
            bunit_gfx_enqueue_textured_quad(
                &rect, 8, 8, 0, g_bunit_gfx_semitrans_enabled, g_bunit_text_digit_texture_page, clut_id, otag_index);
        }
    }

    return desc + desc[1];
}
