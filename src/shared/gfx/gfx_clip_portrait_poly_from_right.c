#include "fft/gfx.h"

/* Trims `amount` texels from the quad's leading edge: the texture origin
 * advances (u0/u2, or v0/v2 when the portrait is not mirrored) and the right
 * edge moves in to match. The screen coordinates are updated as unsigned
 * 16-bit values. */
void gfx_clip_portrait_poly_from_right(POLY_FT4* poly, s32 amount) {
    if (poly->u0 != poly->u1) {
        poly->u0 += amount;
        poly->u2 += amount;
    } else {
        poly->v0 += amount;
        poly->v2 += amount;
    }
    poly->x1 -= amount;
    poly->x3 -= amount;
}
