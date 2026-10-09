#include "fft/world.h"
#include "psx/types.h"

/* Trims `delta` texels from the quad's leading edge: the texture origin
 * advances (u0/u2, or v0/v2 when the portrait is not mirrored) and the right
 * edge moves in to match. The screen coordinates are updated as unsigned
 * halfwords. */
void world_gfx_clip_portrait_poly_from_right(POLY_FT4* poly, s32 delta) {
    if (poly->u0 != poly->u1) {
        poly->u0 += delta;
        poly->u2 += delta;
    } else {
        poly->v0 += delta;
        poly->v2 += delta;
    }
    *(u16*)&poly->x1 -= delta;
    *(u16*)&poly->x3 -= delta;
}
