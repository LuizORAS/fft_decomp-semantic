/* LIBGPU 80023a54-80023a6c. */
#include "psx/libgpu.h"

/* Return the CLUT word for a palette at VRAM (`x`, `y`), x in 16-pixel steps. About 50 functions use it. */
u16 GetClut(int x, int y) {
    return (y << 6) | ((x >> 4) & 0x3f);
}
