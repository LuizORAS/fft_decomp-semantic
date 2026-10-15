/* LIBGPU 80022d10-80022d78. */
#include "psx/libgpu.h"

/* Load a 256-colour CLUT to VRAM (`x`, `y`) and return its CLUT word. WLDCORE's pictures load theirs this
 * way. */
u16 LoadClut(u32* pixels, s32 x, s32 y) {
    RECT rect;
    rect.w = 256;
    rect.x = x;
    rect.y = y;
    rect.h = 1;
    LoadImage(&rect, pixels);
    return GetClut(x, y);
}
