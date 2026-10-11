#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Interpolate the four corners of a textured quad between two RECTs. */
void world_menu_lerp_rect_to_poly_ft4(POLY_FT4* poly, RECT* from, RECT* to, s32 t) {
    setXY4(poly, world_menu_lerp_fixed12(from->x, to->x, t), world_menu_lerp_fixed12(from->y, to->y, t),
        world_menu_lerp_fixed12(from->x + from->w, to->x + to->w, t), world_menu_lerp_fixed12(from->y, to->y, t),
        world_menu_lerp_fixed12(from->x, to->x, t), world_menu_lerp_fixed12(from->y + from->h, to->y + to->h, t),
        world_menu_lerp_fixed12(from->x + from->w, to->x + to->w, t),
        world_menu_lerp_fixed12(from->y + from->h, to->y + to->h, t));
}
