#include "fft/event_attack.h"

/* Build a centered sprite-cell quad using rounded 4.12 fixed-point geometry.
 *
 * This variant selects 8-bit texture mode and adds the PSX screen center to
 * the draw offset. */
void attack_gfx_build_zodiac_sprite_polygon(POLY_FT4* poly, point16_t* texture_page, const RECT* position,
    main_zodiac_sprite_frame_t* sprite_rect, point16_t* scale, const main_zodiac_draw_offset_t* offset) {
    s32 rx;
    s32 ry;
    s32 rw;
    s32 rh;
    s32 ox;
    s32 oy;
    s32 w;
    s32 h;
    s32 x1;
    s32 y1;

    ox = sprite_rect->offset_x * scale->x;
    oy = sprite_rect->offset_y * scale->y;
    w = sprite_rect->width * scale->x;
    h = sprite_rect->height * scale->y;

    rx = 0;
    ry = 0;
    rw = 0;
    rh = 0;
    if ((ox - ((ox / ONE) * ONE)) >= 0x800) {
        rx = 1;
    }
    if ((oy - ((oy / ONE) * ONE)) >= 0x800) {
        ry = 1;
    }
    if ((w - ((w / ONE) * ONE)) >= 0x800) {
        rw = 1;
    }
    if ((h - ((h / ONE) * ONE)) >= 0x800) {
        rh = 1;
    }

    ox = (ox / ONE) + rx;
    oy = (oy / ONE) + ry;
    w = (w / ONE) + rw;
    h = (h / ONE) + rh;
    if (w < 0) {
        w = -w;
    }
    if (h < 0) {
        h = -h;
    }

    poly->tpage = GetTPage(1, 0, texture_page->x, texture_page->y & 0xF00);
    setUV4(poly, sprite_rect->u, sprite_rect->v, sprite_rect->u + sprite_rect->width, sprite_rect->v, sprite_rect->u,
        sprite_rect->v + sprite_rect->height, sprite_rect->u + sprite_rect->width,
        sprite_rect->v + sprite_rect->height);

    x1 = w + 0x100;
    y1 = h + 0x78;

    setXY4(poly, position->x + ox + offset->x + 0x100, position->y + oy + offset->y + 0x78,
        position->x + ox + offset->x + x1, position->y + oy + offset->y + 0x78, position->x + ox + offset->x + 0x100,
        position->y + oy + offset->y + y1, position->x + ox + offset->x + x1, position->y + oy + offset->y + y1);
}
