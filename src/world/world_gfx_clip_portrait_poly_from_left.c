#include "fft/world.h"
#include "psx/types.h"

/* Shifts the quad's left edge right by `delta` and pulls the trailing texture
 * edge back (u1/u3, or v1/v3 when the portrait is not mirrored). The screen
 * coordinates are updated as unsigned halfwords. */
void world_gfx_clip_portrait_poly_from_left(POLY_FT4* poly, s32 delta) {
    poly->x0 += delta;
    poly->x2 += delta;
    if (poly->u0 != poly->u1) {
        poly->u1 -= delta;
        poly->u3 -= delta;
    } else {
        poly->v1 -= delta;
        poly->v3 -= delta;
    }
}
