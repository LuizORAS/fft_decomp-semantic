#include "psx/libgpu.h"

/* Trim `amount` from a portrait quad's right edge and advance its left-hand
 * texture coordinates by the same amount: u when the texture runs along u,
 * otherwise v. */
void attack_gfx_clip_portrait_poly_from_right(POLY_FT4* poly, s32 amount) {
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
