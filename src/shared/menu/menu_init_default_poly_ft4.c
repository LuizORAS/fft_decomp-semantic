#include "fft/menu.h"

/* Initialize the fixed 4-point textured quad used by the menus: a gray quad
 * with all four corners parked off-screen at x -0x200, the 4-bit texture page at
 * x=0x1c0 and CLUT 0x7c3c. BATTLE and WORLD compile this text. */
void menu_init_default_poly_ft4(POLY_FT4* poly) {
    SetPolyFT4(poly);
    SetShadeTex(poly, 0);
    setRGB0(poly, 0x80, 0x80, 0x80);
    setUV4(poly, 0, 0, 1, 0, 0, 1, 1, 1);
    setXY4(poly, -0x200, 0, -0x200, 0, -0x200, 0, -0x200, 0);
    poly->tpage = GetTPage(0, 0, 0x1c0, 0);
    poly->clut = getClut(0x3c0, 0x1f0);
}
