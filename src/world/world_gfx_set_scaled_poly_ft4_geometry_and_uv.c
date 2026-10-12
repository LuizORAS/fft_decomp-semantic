#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Fills one POLY_FT4 for a formation graphic entry: scales the entry's
 * offset and size by a 12-bit fixed scale (rounded to the nearest pixel,
 * size made positive) and places it at position + origin. */
void world_gfx_set_scaled_poly_ft4_geometry_and_uv(POLY_FT4* poly, point16_t* page, const RECT* position,
    sprite_sheet_frame_t* rect, point16_t* scale, sprite_draw_offset_t* origin) {
    s32 x_off = rect->offset.x * scale->x;
    s32 y_off = rect->offset.y * scale->y;
    s32 w = rect->source.w * scale->x;
    s32 h = rect->source.h * scale->y;
    s32 round_x = 0;
    s32 round_y = 0;
    s32 round_w = 0;
    s32 round_h = 0;
    s32 whole_x;
    s32 whole_y;
    s32 whole_w;
    s32 whole_h;

    whole_x = x_off / ONE;
    if (x_off - whole_x * ONE >= 0x800) {
        round_x = 1;
    }
    whole_y = y_off / ONE;
    if (y_off - whole_y * ONE >= 0x800) {
        round_y = 1;
    }
    whole_w = w / ONE;
    if (w - whole_w * ONE >= 0x800) {
        round_w = 1;
    }
    whole_h = h / ONE;
    if (h - whole_h * ONE >= 0x800) {
        round_h = 1;
    }
    x_off = whole_x + round_x;
    y_off = whole_y + round_y;
    w = whole_w + round_w;
    h = whole_h + round_h;
    if (w < 0) {
        w = -w;
    }
    if (h < 0) {
        h = -h;
    }
    poly->tpage = GetTPage(1, 0, page->x, page->y & 0xF00);
    setUV4(poly, rect->source.x, rect->source.y, rect->source.x + rect->source.w, rect->source.y, rect->source.x,
        rect->source.y + rect->source.h, rect->source.x + rect->source.w, rect->source.y + rect->source.h);
    setXY4(poly, (u16)position->x + x_off + origin->position.x, (u16)position->y + y_off + origin->position.y,
        (u16)position->x + x_off + origin->position.x + w, (u16)position->y + y_off + origin->position.y,
        (u16)position->x + x_off + origin->position.x, (u16)position->y + y_off + origin->position.y + h,
        (u16)position->x + x_off + origin->position.x + w, (u16)position->y + y_off + origin->position.y + h);
}
