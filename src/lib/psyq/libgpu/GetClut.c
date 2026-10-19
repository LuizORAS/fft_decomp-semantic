/* LIBGPU 80023a54-80023a6c. */
#include "psx/libgpu.h"

/* Return the CLUT id for a palette at VRAM (`x`, `y`), x in 16-pixel steps. About 50 functions use it. */
u16 GetClut(int x, int y) {
    return getClut(x, y);
}
