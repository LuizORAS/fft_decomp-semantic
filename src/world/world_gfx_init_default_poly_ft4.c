#include "fft/world.h"
#include "psx/libgpu.h"

/* Initialize the fixed 4-point textured quad used by WORLD menus: a
 * 0x200-wide grey quad parked off-screen to the left, texture page 0
 * at x=0x1c0, CLUT 0x7c3c. */
void world_gfx_init_default_poly_ft4(POLY_FT4* poly) {
    SetPolyFT4(poly);
    SetShadeTex(poly, 0);
    setRGB0(poly, 0x80, 0x80, 0x80);
    poly->u1 = 1;
    poly->v2 = 1;
    poly->u3 = 1;
    poly->v3 = 1;
    poly->u0 = 0;
    poly->v0 = 0;
    poly->v1 = 0;
    poly->u2 = 0;
    setXY4(poly, -0x200, 0, -0x200, 0, -0x200, 0, -0x200, 0);
    poly->tpage = GetTPage(0, 0, 0x1c0, 0);
    poly->clut = 0x7c3c;
}
