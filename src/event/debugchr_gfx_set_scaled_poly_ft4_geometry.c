#include "fft/event_debugchr.h"
#include "psx/libgpu.h"

/* Configure a sprite quad using truncated 4.12 fixed-point geometry. */
void debugchr_gfx_set_scaled_poly_ft4_geometry(POLY_FT4* poly, const point16_t* texture_page, const RECT* position,
    main_zodiac_sprite_frame_t* sprite_rect, point16_t* scale, main_zodiac_draw_offset_t* base) {
    s16 scale_x;
    s16 scale_y;
    s32 dx;
    s32 dy;
    s32 w;
    s32 h;

    scale_x = scale->x;
    dx = (sprite_rect->offset_x * scale_x) / ONE;
    scale_y = scale->y;
    dy = (sprite_rect->offset_y * scale_y) / ONE;
    w = (sprite_rect->width * scale_x) / ONE;
    h = (sprite_rect->height * scale_y) / ONE;
    if (w < 0) {
        w = -w;
    }
    if (h < 0) {
        h = -h;
    }

    poly->tpage = GetTPage(0, 0, texture_page->x, texture_page->y & 0xf00);
    poly->u0 = sprite_rect->u;
    poly->v0 = sprite_rect->v;
    poly->u1 = sprite_rect->u + sprite_rect->width;
    poly->v1 = sprite_rect->v;
    poly->u2 = sprite_rect->u;
    poly->v2 = sprite_rect->v + sprite_rect->height;
    poly->u3 = sprite_rect->u + sprite_rect->width;
    poly->v3 = sprite_rect->v + sprite_rect->height;
    poly->x0 = position->x + dx + base->x;
    poly->y0 = position->y + dy + base->y;
    poly->x1 = position->x + dx + base->x + w;
    poly->y1 = position->y + dy + base->y;
    poly->x2 = position->x + dx + base->x;
    poly->y2 = position->y + dy + base->y + h;
    poly->x3 = position->x + dx + base->x + w;
    poly->y3 = position->y + dy + base->y + h;
}
