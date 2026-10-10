#include "psx/libgpu.h"
#include "psx/types.h"

/* Shift a portrait quad's left edge right by `amount` and trim the same amount
 * from its right-hand texture coordinates: u when the texture runs along u,
 * otherwise v. */
void equip_gfx_clip_portrait_poly_from_left(POLY_FT4* poly, s32 amount) {
    /* x0/x2 are s16 in POLY_FT4 but the target loads them lhu. */
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
