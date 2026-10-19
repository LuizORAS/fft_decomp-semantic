#include "fft/menu.h"

/* Set a quad's corners to the rectangle interpolated between from and to by a 12-bit fraction. WORLD
 * compiles this text too. */
void menu_lerp_rect_to_poly_ft4(POLY_FT4* poly, const RECT* from, const RECT* to, s32 factor) {
    setXY4(poly, menu_lerp_fixed12(from->x, to->x, factor), menu_lerp_fixed12(from->y, to->y, factor),
        menu_lerp_fixed12(from->x + from->w, to->x + to->w, factor), menu_lerp_fixed12(from->y, to->y, factor),
        menu_lerp_fixed12(from->x, to->x, factor), menu_lerp_fixed12(from->y + from->h, to->y + to->h, factor),
        menu_lerp_fixed12(from->x + from->w, to->x + to->w, factor),
        menu_lerp_fixed12(from->y + from->h, to->y + to->h, factor));
}
