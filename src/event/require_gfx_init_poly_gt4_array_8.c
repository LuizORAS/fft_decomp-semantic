#include "fft/event_require.h"

/* Initialise the 8 semi-transparent POLY_GT4s of g_require_gfx_poly_gt4_array_8
 * with zeroed vertices, a one-texel texture cell, texture page 6 and the CLUT
 * at the bonus palette rectangle's origin offset by (x, y). */
void require_gfx_init_poly_gt4_array_8(s32 x, s32 y) {
    s32 count;
    s32 enabled;
    s16* palette_origin;

    count = 0;
    enabled = 1;
    /* The y-centered pointer preserves the retail base address; RECT fields change it. */
    palette_origin = &g_require_reward_bonus_palette_rect.y;
    do {
        POLY_GT4* poly = &g_require_gfx_poly_gt4_array_8[count];
        SetPolyGT4(poly);
        SetShadeTex(poly, 0);
        SetSemiTrans(poly, 1);
        poly->u0 = 0;
        poly->v0 = 0;
        poly->u1 = enabled;
        poly->v1 = 0;
        poly->u2 = 0;
        poly->v2 = enabled;
        poly->u3 = enabled;
        poly->v3 = enabled;
        poly->x0 = 0;
        poly->y0 = 0;
        poly->x1 = 0;
        poly->y1 = 0;
        poly->x2 = 0;
        poly->y2 = 0;
        poly->x3 = 0;
        poly->y3 = 0;
        if (GetGraphType() != enabled) {
            GetGraphType();
        }
        count++;
        poly->tpage = 6;
        poly->clut = getClut(palette_origin[-1] + x, palette_origin[0] + y);
    } while (count < 8);
}
