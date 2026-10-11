#include "fft/event_jobstts.h"

/* Takes one POLY_FT4 from the pool, fills it as the rect->w x rect->h quad at
 * (rect->x + 0x80, rect->y) textured with the icon source's cell, CLUT and
 * texture page, and links it into ordering table entry ot_index. With
 * `color` the quad is shaded with it. */
void jobstts_gfx_enqueue_textured_quad_rect(
    RECT* rect, world_item_icon_source_t* tex, rgb24_t* color, s32 semitrans, s32 ot_index) {
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

    setXYWH(poly, rect->x + 0x80, rect->y, rect->w, rect->h);
    setUVWH(poly, tex->u, tex->v, tex->w, tex->h);
    poly->clut = tex->clut;
    poly->tpage = tex->tpage;

    setaddr(poly, g_jobstts_gfx_context->otag[ot_index]);
    catPrim(&g_jobstts_gfx_context->otag[ot_index], poly);
}
