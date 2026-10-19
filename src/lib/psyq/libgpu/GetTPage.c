/* LIBGPU 8002398c-80023a54. */
#include "psx/libgpu.h"

/* Return the texture page id for colour depth `tp`, semi-transparency mode `abr` and the VRAM page holding
 * (`x`, `y`), in the standard or extended GPU's layout. About 135 functions use it. */
u16 GetTPage(int tp, int abr, int x, int y) {
    return getTPage(tp, abr, x, y);
}
