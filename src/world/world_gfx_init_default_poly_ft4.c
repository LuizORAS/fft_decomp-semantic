#include "fft/world.h"
#include "psx/libgpu.h"

/* Initialize the fixed 4-point textured quad used by WORLD menus: a
 * 0x200-wide grey quad parked off-screen to the left, texture page 0
 * at x=0x1c0, CLUT 0x7c3c. */
void world_gfx_init_default_poly_ft4(POLY_FT4* poly) {
    SetPolyFT4(poly);
    SetShadeTex(poly, 0);
    setRGB0(poly, 0x80, 0x80, 0x80);
    setUV4(poly, 0, 0, 1, 0, 0, 1, 1, 1);
    setXY4(poly, -0x200, 0, -0x200, 0, -0x200, 0, -0x200, 0);
    poly->tpage = GetTPage(0, 0, 0x1c0, 0);
    poly->clut = 0x7c3c;
}
