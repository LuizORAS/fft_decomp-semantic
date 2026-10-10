#include "psx/libgpu.h"

/* Set `rect` to the 16x1 VRAM strip a CLUT id names (x = low six bits * 16,
 * y = the bits above them). */
void debugchr_gfx_set_clut_rect_from_id(RECT* rect, s32 packed) {
    s16 x;
    s16 y;

    x = (packed & 0x3f) << 4;
    y = (packed & 0xffff) >> 6;
    rect->x = x;
    rect->w = 0x10;
    rect->y = y;
    rect->h = 1;
}
