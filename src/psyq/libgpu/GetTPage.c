/* LIBGPU 8002398c-80023a54. */
#include "psx/libgpu.h"

/* Return the texture page word for colour depth `tp`, semi-transparency mode `abr` and the VRAM page holding
 * (`x`, `y`), in the standard or extended GPU's layout. About 135 functions use it. */
u16 GetTPage(int tp, int abr, int x, int y) {
    u32 result;
    if (GetGraphType() == 1 || GetGraphType() == 2) {
        result = ((tp & 3) << 9) | ((abr & 3) << 7) | ((y & 0x300) >> 3) | ((x & 0x3ff) >> 6);
    } else {
        result = ((tp & 3) << 7) | ((abr & 3) << 5) | ((y & 0x100) >> 4) | ((x & 0x3ff) >> 6) | ((y & 0x200) << 2);
    }
    return result;
}
