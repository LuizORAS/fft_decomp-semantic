#include "fft/battle.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Fills one POLY_GT4 per piece of quad group `group` and returns the piece count.
 *
 * BATTLE twin of world_menu_build_quad_pieces. */
s32 battle_menu_build_quad_pieces(s32 group, POLY_GT4* poly) {
    world_menu_quad_piece_t* piece;
    s32 count;
    s32 i;
    world_menu_quad_colors_t* colors;

    colors = &g_battle_menu_quad_colors;
    piece = &g_battle_menu_quad_pieces[g_battle_menu_quad_group_starts[group]];
    count = g_battle_menu_quad_group_starts[group + 1] - g_battle_menu_quad_group_starts[group];
    for (i = 0; i < count; i++) {
        poly->x0 = g_battle_menu_quad_origin.x + piece->x;
        poly->y0 = g_battle_menu_quad_origin.y + piece->y;
        poly->x1 = g_battle_menu_quad_origin.x + piece->x + piece->w;
        poly->y1 = g_battle_menu_quad_origin.y + piece->y;
        poly->x2 = g_battle_menu_quad_origin.x + piece->x;
        poly->y2 = g_battle_menu_quad_origin.y + piece->y + (piece->h - 1);
        poly->x3 = g_battle_menu_quad_origin.x + piece->x + piece->w;
        poly->y3 = g_battle_menu_quad_origin.y + piece->y + (piece->h - 1);
        poly->u0 = piece->u;
        poly->v0 = piece->v - 0x78;
        poly->u1 = piece->u + piece->w;
        poly->v1 = piece->v - 0x78;
        poly->u2 = piece->u;
        poly->v2 = piece->v - 0x78 + (piece->h - 1);
        poly->u3 = piece->u + piece->w;
        poly->v3 = piece->v - 0x78 + (piece->h - 1);
        setRGB0(poly, colors->vertex[0].r, colors->vertex[0].g, colors->vertex[0].b);
        setRGB1(poly, colors->vertex[1].r, colors->vertex[1].g, colors->vertex[1].b);
        setRGB2(poly, colors->vertex[2].r, colors->vertex[2].g, colors->vertex[2].b);
        setRGB3(poly, colors->vertex[3].r, colors->vertex[3].g, colors->vertex[3].b);
        poly++;
        piece++;
    }
    return count;
}
