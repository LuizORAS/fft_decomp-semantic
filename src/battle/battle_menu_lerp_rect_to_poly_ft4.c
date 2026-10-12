#include "fft/battle_menu.h"
#include "fft/menu.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Set a quad's corners to the rectangle interpolated between from and to by a 12-bit fraction. */
void battle_menu_lerp_rect_to_poly_ft4(POLY_FT4* poly, RECT* from, RECT* to, s32 factor) {
    setXY4(poly, menu_lerp_fixed12(from->x, to->x, factor), menu_lerp_fixed12(from->y, to->y, factor),
        menu_lerp_fixed12(from->x + from->w, to->x + to->w, factor), menu_lerp_fixed12(from->y, to->y, factor),
        menu_lerp_fixed12(from->x, to->x, factor), menu_lerp_fixed12(from->y + from->h, to->y + to->h, factor),
        menu_lerp_fixed12(from->x + from->w, to->x + to->w, factor),
        menu_lerp_fixed12(from->y + from->h, to->y + to->h, factor));
}
