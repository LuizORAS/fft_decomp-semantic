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
        setRGB0(poly, color->r, color->g, color->b);
    } else {
        SetShadeTex(poly, 1);
    }
    SetSemiTrans(poly, semitrans);

    setXY4(poly, quad->x0 + 0x80, quad->y0, quad->x1 + 0x80, quad->y1, quad->x2 + 0x80, quad->y2, quad->x3 + 0x80,
        quad->y3);
    setUV4(poly, quad->u0, quad->v0, quad->u1, quad->v1, quad->u2, quad->v2, quad->u3, quad->v3);
    poly->clut = quad->clut;
    poly->tpage = quad->tpage;

    setaddr(poly, g_jobstts_gfx_context->otag[ot_index]);
    catPrim(&g_jobstts_gfx_context->otag[ot_index], poly);
}
