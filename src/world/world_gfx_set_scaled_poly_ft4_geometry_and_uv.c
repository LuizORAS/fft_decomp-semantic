#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Fills one POLY_FT4 for a formation graphic entry: scales the entry's
 * offset and size by a 12-bit fixed scale (rounded to the nearest pixel,
 * size made positive) and places it at position + origin. */
void world_gfx_set_scaled_poly_ft4_geometry_and_uv(POLY_FT4* poly, point16_t* page, const RECT* position,
    main_zodiac_sprite_frame_t* rect, point16_t* scale, main_zodiac_draw_offset_t* origin) {
    s32 x_off = rect->offset_x * scale->x;
    s32 y_off = rect->offset_y * scale->y;
    s32 w = rect->width * scale->x;
    s32 h = rect->height * scale->y;
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
    poly->u0 = rect->u;
    poly->v0 = rect->v;
    poly->u1 = rect->u + rect->width;
    poly->v1 = rect->v;
    poly->u2 = rect->u;
    poly->v2 = rect->v + rect->height;
    poly->u3 = rect->u + rect->width;
    poly->v3 = rect->v + rect->height;
    poly->x0 = (u16)position->x + x_off + origin->x;
    poly->y0 = (u16)position->y + y_off + origin->y;
    poly->x1 = (u16)position->x + x_off + origin->x + w;
    poly->y1 = (u16)position->y + y_off + origin->y;
    poly->x2 = (u16)position->x + x_off + origin->x;
    poly->y2 = (u16)position->y + y_off + origin->y + h;
    poly->x3 = (u16)position->x + x_off + origin->x + w;
    poly->y3 = (u16)position->y + y_off + origin->y + h;
}
