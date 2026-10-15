/* LIBGPU 80022d78-80022de0. */
#include "psx/libgpu.h"

/* Load a 16-colour CLUT to VRAM (`x`, `y`) and return its CLUT word. FntLoad and MAIN's font and NOW LOADING
 * images use it. */
u16 LoadClut2(u32* pixels, int x, int y) {
    RECT rect;
    rect.w = 16;
    rect.x = x;
    rect.y = y;
    rect.h = 1;
    LoadImage(&rect, pixels);
    return GetClut(x, y);
}
