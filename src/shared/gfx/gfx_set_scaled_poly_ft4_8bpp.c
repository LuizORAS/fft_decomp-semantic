#include "fft/gfx.h"

/* Fill one POLY_FT4 for a sprite-sheet frame at 8 bits per pixel: scale the frame's offset and size
 * by a 12-bit fixed scale (rounded up from half a pixel when the fraction is positive; the size made
 * positive) and place it at position + offset, on the 8-bit texture page holding `texture`. CARD,
 * DEBUGCHR and REQUIRE compile this text, a copy of gfx_set_scaled_poly_ft4_8bpp. */
void gfx_set_scaled_poly_ft4_8bpp(POLY_FT4* poly, const point16_t* texture, const RECT* position,
    const sprite_sheet_frame_t* uv_rect, const point16_t* scale, const main_zodiac_draw_offset_t* offset) {
    s32 xoff = uv_rect->offset.x * scale->x;
    s32 yoff = uv_rect->offset.y * scale->y;
    s32 width = uv_rect->source.w * scale->x;
    s32 height = uv_rect->source.h * scale->y;
    s32 round_xoff = 0;
    s32 round_yoff = 0;
    s32 round_width = 0;
    s32 round_height = 0;
    s32 xoff_quotient;
    s32 yoff_quotient;
    s32 width_quotient;
    s32 height_quotient;

    xoff_quotient = xoff / ONE;
    if (xoff % ONE >= ONE / 2)
        round_xoff = 1;
    yoff_quotient = yoff / ONE;
    if (yoff % ONE >= ONE / 2)
        round_yoff = 1;
    width_quotient = width / ONE;
    if (width % ONE >= ONE / 2)
        round_width = 1;
    height_quotient = height / ONE;
    if (height % ONE >= ONE / 2)
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
    setUV4(poly, uv_rect->source.x, uv_rect->source.y, uv_rect->source.x + uv_rect->source.w, uv_rect->source.y,
        uv_rect->source.x, uv_rect->source.y + uv_rect->source.h, uv_rect->source.x + uv_rect->source.w,
        uv_rect->source.y + uv_rect->source.h);
    setXY4(poly, position->x + xoff + offset->position.x, position->y + yoff + offset->position.y,
        position->x + xoff + offset->position.x + width, position->y + yoff + offset->position.y,
        position->x + xoff + offset->position.x, position->y + yoff + offset->position.y + height,
        position->x + xoff + offset->position.x + width, position->y + yoff + offset->position.y + height);
}
