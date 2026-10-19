#include "fft/main.h"
#include "psx/libgte.h"

/* Fill one textured quad from a zodiac sprite record.
 *
 * The record's offset and size are multiplied by scale (ONE = 1.0) and rounded to
 * nearest; a negative size is made positive. The quad's top-left corner is base +
 * offset + the scaled record offset + (0x100, 0x78), and it samples the unscaled
 * record rectangle from the 8-bit texture page that holds *texture. */
void main_gfx_build_zodiac_sprite_polygon(POLY_FT4* poly, point16_t* texture, upoint16_t* base,
    sprite_sheet_frame_t* frame, point16_t* scale, main_zodiac_draw_offset_t* offset) {
    s32 x = frame->offset.x * scale->x;
    s32 y = frame->offset.y * scale->y;
    s32 width = frame->source.w * scale->x;
    s32 height = frame->source.h * scale->y;
    s32 round_x = 0;
    s32 round_y = 0;
    s32 round_width = 0;
    s32 round_height = 0;
    s32 qx;
    s32 qy;
    s32 qw;
    s32 qh;
    s32 right;
    s32 bottom;

    qx = x / ONE;
    if (x - qx * ONE >= 0x800) {
        round_x = 1;
    }
    qy = y / ONE;
    if (y - qy * ONE >= 0x800) {
        round_y = 1;
    }
    qw = width / ONE;
    if (width - qw * ONE >= 0x800) {
        round_width = 1;
    }
    qh = height / ONE;
    if (height - qh * ONE >= 0x800) {
        round_height = 1;
    }
    x = qx + round_x;
    y = qy + round_y;
    width = qw + round_width;
    height = qh + round_height;
    if (width < 0) {
        width = -width;
    }
    if (height < 0) {
        height = -height;
    }
    poly->tpage = GetTPage(1, 0, texture->x, texture->y & 0xF00);
    poly->u0 = frame->source.x;
    poly->v0 = frame->source.y;
    /* The target adds only the low byte of the 16-bit width/height here. */
    poly->u1 = frame->source.x + frame->source.w;
    poly->v1 = frame->source.y;
    poly->u2 = frame->source.x;
    poly->v2 = frame->source.y + frame->source.h;
    poly->u3 = frame->source.x + frame->source.w;
    poly->v3 = frame->source.y + frame->source.h;
    right = width + 0x100;
    bottom = height + 0x78;
    setXY4(poly, base->x + x + offset->position.x + 0x100, base->y + y + offset->position.y + 0x78,
        base->x + x + offset->position.x + right, base->y + y + offset->position.y + 0x78,
        base->x + x + offset->position.x + 0x100, base->y + y + offset->position.y + bottom,
        base->x + x + offset->position.x + right, base->y + y + offset->position.y + bottom);
}
