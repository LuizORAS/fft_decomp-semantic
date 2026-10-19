#include "fft/event_jobstts.h"

/* Takes one POLY_FT4 from the pool, fills it as the rect->w x rect->h quad at
 * (rect->x + 0x80, rect->y) with texture origin (u, v), and links it into
 * ordering table entry ot_index. With `color` the quad is shaded with it. */
void jobstts_gfx_enqueue_textured_quad(
    const RECT* rect, s32 u, s32 v, const rgb24_t* color, s32 semitrans, u16 tpage, u16 clut, s32 ot_index) {
    POLY_FT4* poly;

    poly = &g_jobstts_gfx_context->textured_quads[g_jobstts_gfx_poly_ft4_count++];

    if (color != 0) {
        SetShadeTex(poly, 0);
        setRGB0(poly, color->r, color->g, color->b);
    } else {
        SetShadeTex(poly, 1);
    }
    SetSemiTrans(poly, semitrans);

    setXYWH(poly, rect->x + 0x80, rect->y, rect->w, rect->h);
    setUVWH(poly, u, v, rect->w, rect->h);
    poly->clut = clut;
    poly->tpage = tpage;

    setaddr(poly, g_jobstts_gfx_context->otag[ot_index]);
    catPrim(&g_jobstts_gfx_context->otag[ot_index], poly);
}
