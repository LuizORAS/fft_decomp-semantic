#include "fft/gfx.h"

/* Shifts the quad's left edge right by `amount` and pulls the trailing texture
 * edge back (u1/u3, or v1/v3 when the portrait is not mirrored). The screen
 * coordinates are updated as unsigned 16-bit values. */
void gfx_clip_portrait_poly_from_left(POLY_FT4* poly, s32 amount) {
    poly->x0 += amount;
    poly->x2 += amount;
    if (poly->u0 != poly->u1) {
        poly->u1 -= amount;
        poly->u3 -= amount;
    } else {
        poly->v1 -= amount;
        poly->v3 -= amount;
    }
}
