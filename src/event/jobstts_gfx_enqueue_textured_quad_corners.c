#include "fft/event_jobstts.h"

/*
 * Append a textured menu quad, given by its four corners, to the selected
 * ordering-table entry.
 *
 * Screen x coordinates use the menu's +0x80 origin. With `color` the quad is
 * shaded with that color; otherwise texture shading is disabled.
 */
void jobstts_gfx_enqueue_textured_quad_corners(
    world_gfx_textured_quad_source_t* quad, rgb24_t* color, s32 semitrans, s32 ot_index) {
    POLY_FT4* poly;

    poly = &g_jobstts_gfx_context->textured_quads[g_jobstts_gfx_poly_ft4_count++];

    if (color != 0) {
        SetShadeTex(poly, 0);
        poly->r0 = color->r;
        poly->g0 = color->g;
        poly->b0 = color->b;
    } else {
        SetShadeTex(poly, 1);
    }
    SetSemiTrans(poly, semitrans);

    poly->x0 = quad->x0 + 0x80;
    poly->y0 = quad->y0;
    poly->x1 = quad->x1 + 0x80;
    poly->y1 = quad->y1;
    poly->x2 = quad->x2 + 0x80;
    poly->y2 = quad->y2;
    poly->x3 = quad->x3 + 0x80;
    poly->y3 = quad->y3;
    poly->u0 = quad->u0;
    poly->v0 = quad->v0;
    poly->u1 = quad->u1;
    poly->v1 = quad->v1;
    poly->u2 = quad->u2;
    poly->v2 = quad->v2;
    poly->u3 = quad->u3;
    poly->v3 = quad->v3;
    poly->clut = quad->clut;
    poly->tpage = quad->tpage;

    setaddr(poly, g_jobstts_gfx_context->otag[ot_index]);
    catPrim(&g_jobstts_gfx_context->otag[ot_index], poly);
}
