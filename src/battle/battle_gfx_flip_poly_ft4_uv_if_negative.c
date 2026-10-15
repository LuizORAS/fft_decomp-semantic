#include "psx/libgpu.h"
#include "psx/types.h"

/* Mirrors a quad's texture when direction is negative: corners 0 and 1 and
 * corners 2 and 3 swap their u when corners 0 and 1 differ in u, otherwise they
 * swap their v. BATTLE twin of world_gfx_flip_poly_ft4_uv_if_negative; the window
 * drawing uses it for a flipped portrait. */
void battle_gfx_flip_poly_ft4_uv_if_negative(POLY_FT4* primitive, s32 direction) {
    u8 temp;

    if (direction < 0) {
        if (primitive->u0 != primitive->u1) {
            temp = primitive->u0;
            primitive->u0 = primitive->u1;
            primitive->u1 = temp;
            temp = primitive->u2;
            primitive->u2 = primitive->u3;
            primitive->u3 = temp;
        } else {
            temp = primitive->v0;
            primitive->v0 = primitive->v1;
            primitive->v1 = temp;
            temp = primitive->v2;
            primitive->v2 = primitive->v3;
            primitive->v3 = temp;
        }
    }
}
