#include "fft/world.h"
#include "psx/libgpu.h"

/* Set `rect` to the 16x1 VRAM strip a CLUT id names (x = low six bits * 16,
 * y = the bits above them). */
void world_gfx_set_clut_rect_from_id(RECT* rect, u16 clut_id) {
    s16 x;

    x = (clut_id & 0x3f) << 4;
    clut_id >>= 6;
    rect->x = x;
    rect->w = 0x10;
    rect->y = clut_id;
    rect->h = 1;
}
