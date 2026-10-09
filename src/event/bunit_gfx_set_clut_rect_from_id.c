#include "psx/libgpu.h"

void bunit_gfx_set_clut_rect_from_id(RECT* rect, s32 clut_id) {
    s16 x;
    s16 y;

    x = (clut_id & 0x3f) << 4;
    y = (clut_id & 0xffff) >> 6;
    rect->x = x;
    rect->w = 0x10;
    rect->y = y;
    rect->h = 1;
}
