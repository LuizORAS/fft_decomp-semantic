#include "fft/battle.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Set a quad's corners to the rectangle interpolated between from and to by a 12-bit fraction. */
void battle_menu_lerp_rect_to_poly_ft4(POLY_FT4* poly, RECT* from, RECT* to, s32 factor) {
    setXY4(poly, battle_menu_lerp_fixed12(from->x, to->x, factor), battle_menu_lerp_fixed12(from->y, to->y, factor),
        battle_menu_lerp_fixed12(from->x + from->w, to->x + to->w, factor),
        battle_menu_lerp_fixed12(from->y, to->y, factor), battle_menu_lerp_fixed12(from->x, to->x, factor),
        battle_menu_lerp_fixed12(from->y + from->h, to->y + to->h, factor),
        battle_menu_lerp_fixed12(from->x + from->w, to->x + to->w, factor),
        battle_menu_lerp_fixed12(from->y + from->h, to->y + to->h, factor));
}
