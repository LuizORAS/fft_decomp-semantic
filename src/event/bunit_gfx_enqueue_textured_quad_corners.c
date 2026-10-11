#include "fft/event_bunit.h"
#include "psx/types.h"

/*
 * Append a textured menu quad, given by its four corners, to the selected
 * ordering-table entry.
 *
 * Screen x coordinates use the menu's +0x80 origin. With `color` the quad is
 * shaded with that color; otherwise texture shading is disabled.
 */
void bunit_gfx_enqueue_textured_quad_corners(world_gfx_textured_quad_source_t* src, rgb24_t* color, s32 semi, s32 idx) {
    POLY_FT4* poly;

    poly = &g_bunit_gfx_context->textured_quads[g_bunit_gfx_poly_ft4_count++];
    if (color != 0) {
        SetShadeTex(poly, 0);
        poly->r0 = color->r;
        poly->g0 = color->g;
        poly->b0 = color->b;
    } else {
        SetShadeTex(poly, 1);
    }
    SetSemiTrans(poly, semi);
    poly->x0 = src->x0 + 0x80;
    poly->y0 = src->y0;
    poly->x1 = src->x1 + 0x80;
    poly->y1 = src->y1;
    poly->x2 = src->x2 + 0x80;
    poly->y2 = src->y2;
    poly->x3 = src->x3 + 0x80;
    poly->y3 = src->y3;
    poly->u0 = src->u0;
    poly->v0 = src->v0;
    poly->u1 = src->u1;
    poly->v1 = src->v1;
    poly->u2 = src->u2;
    poly->v2 = src->v2;
    poly->u3 = src->u3;
    poly->v3 = src->v3;
    poly->clut = src->clut;
    poly->tpage = src->tpage;
    addPrim(&g_bunit_gfx_context->otag[idx], poly);
}
