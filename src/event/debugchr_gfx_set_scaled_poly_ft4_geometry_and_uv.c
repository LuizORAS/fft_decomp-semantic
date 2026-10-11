#include "fft/event_debugchr.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Fills one POLY_FT4 for a sprite-sheet frame: scales the frame's offset and
 * size by a 12-bit fixed scale (rounded to the nearest pixel, size made
 * positive) and places it at position + offset. The texture page is the
 * 8-bit page holding `texture`. DEBUGCHR copy of
 * world_gfx_set_scaled_poly_ft4_geometry_and_uv. */
void debugchr_gfx_set_scaled_poly_ft4_geometry_and_uv(POLY_FT4* poly, const point16_t* texture, const RECT* position,
    const main_zodiac_sprite_frame_t* uv_rect, const point16_t* scale, const main_zodiac_draw_offset_t* offset) {
    s32 xoff = uv_rect->offset_x * scale->x;
    s32 yoff = uv_rect->offset_y * scale->y;
    s32 width = uv_rect->width * scale->x;
    s32 height = uv_rect->height * scale->y;
    s32 round_xoff = 0;
    s32 round_yoff = 0;
    s32 round_width = 0;
    s32 round_height = 0;
    s32 xoff_quotient;
    s32 yoff_quotient;
    s32 width_quotient;
    s32 height_quotient;

    xoff_quotient = xoff / ONE;
    if (xoff - xoff_quotient * ONE >= 2048)
        round_xoff = 1;
    yoff_quotient = yoff / ONE;
    if (yoff - yoff_quotient * ONE >= 2048)
        round_yoff = 1;
    width_quotient = width / ONE;
    if (width - width_quotient * ONE >= 2048)
        round_width = 1;
    height_quotient = height / ONE;
    if (height - height_quotient * ONE >= 2048)
        round_height = 1;
    xoff = xoff_quotient + round_xoff;
    yoff = yoff_quotient + round_yoff;
    width = width_quotient + round_width;
    height = height_quotient + round_height;
    if (width < 0)
        width = -width;
    if (height < 0)
        height = -height;

    poly->tpage = GetTPage(1, 0, texture->x, texture->y & 0xf00);
    setUV4(poly, uv_rect->u, uv_rect->v, uv_rect->u + uv_rect->width, uv_rect->v, uv_rect->u,
        uv_rect->v + uv_rect->height, uv_rect->u + uv_rect->width, uv_rect->v + uv_rect->height);
    setXY4(poly, position->x + xoff + offset->x, position->y + yoff + offset->y, position->x + xoff + offset->x + width,
        position->y + yoff + offset->y, position->x + xoff + offset->x, position->y + yoff + offset->y + height,
        position->x + xoff + offset->x + width, position->y + yoff + offset->y + height);
}
