#include "fft/event_debugchr.h"
#include "psx/libgpu.h"

/* Configure a sprite quad using truncated 4.12 fixed-point geometry. */
void debugchr_gfx_set_scaled_poly_ft4_geometry(POLY_FT4* poly, const point16_t* texture_page, const RECT* position,
    sprite_sheet_frame_t* sprite_rect, point16_t* scale, sprite_draw_offset_t* base) {
    s16 scale_x;
    s16 scale_y;
    s32 dx;
    s32 dy;
    s32 w;
    s32 h;

    scale_x = scale->x;
    dx = (sprite_rect->offset.x * scale_x) / ONE;
    scale_y = scale->y;
    dy = (sprite_rect->offset.y * scale_y) / ONE;
    w = (sprite_rect->source.w * scale_x) / ONE;
    h = (sprite_rect->source.h * scale_y) / ONE;
    if (w < 0) {
        w = -w;
    }
    if (h < 0) {
        h = -h;
    }

    poly->tpage = GetTPage(0, 0, texture_page->x, texture_page->y & 0xf00);
    setUV4(poly, sprite_rect->source.x, sprite_rect->source.y, sprite_rect->source.x + sprite_rect->source.w,
        sprite_rect->source.y, sprite_rect->source.x, sprite_rect->source.y + sprite_rect->source.h,
        sprite_rect->source.x + sprite_rect->source.w, sprite_rect->source.y + sprite_rect->source.h);
    setXY4(poly, position->x + dx + base->position.x, position->y + dy + base->position.y,
        position->x + dx + base->position.x + w, position->y + dy + base->position.y,
        position->x + dx + base->position.x, position->y + dy + base->position.y + h,
        position->x + dx + base->position.x + w, position->y + dy + base->position.y + h);
}
