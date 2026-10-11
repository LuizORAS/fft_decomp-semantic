#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Interpolate the four corners of a textured quad between two RECTs. The menu-screen library's copy of
 * world_menu_lerp_rect_to_poly_ft4; world_menu_animate_window_quads_to_rects calls this one. */
void world_screen_menu_lerp_rect_to_poly_ft4(POLY_FT4* poly, RECT* from, RECT* to, s32 t) {
    poly->x0 = world_screen_menu_lerp_fixed12(from->x, to->x, t);
    poly->y0 = world_screen_menu_lerp_fixed12(from->y, to->y, t);
    poly->x1 = world_screen_menu_lerp_fixed12(from->x + from->w, to->x + to->w, t);
    poly->y1 = world_screen_menu_lerp_fixed12(from->y, to->y, t);
    poly->x2 = world_screen_menu_lerp_fixed12(from->x, to->x, t);
    poly->y2 = world_screen_menu_lerp_fixed12(from->y + from->h, to->y + to->h, t);
    poly->x3 = world_screen_menu_lerp_fixed12(from->x + from->w, to->x + to->w, t);
    poly->y3 = world_screen_menu_lerp_fixed12(from->y + from->h, to->y + to->h, t);
}
