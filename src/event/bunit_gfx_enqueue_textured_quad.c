#include "fft/event_bunit.h"
#include "psx/types.h"

/* Takes one POLY_FT4 from the pool, fills it as the rect->w x rect->h quad at
 * (rect->x + 0x80, rect->y) with texture origin (u, v), and links it into
 * ordering table entry otag_index. With `color` the quad is shaded with it. */
void bunit_gfx_enqueue_textured_quad(
    const RECT* rect, s32 u, s32 v, const u8* color, s32 semitrans, u16 texture_page, u16 clut, s32 otag_index) {
    POLY_FT4* poly;

    poly = &g_bunit_gfx_context->textured_quads[g_bunit_gfx_poly_ft4_count++];

    if (color != 0) {
        SetShadeTex(poly, 0);
        poly->r0 = color[0];
        poly->g0 = color[1];
        poly->b0 = color[2];
    } else {
        SetShadeTex(poly, 1);
    }
    SetSemiTrans(poly, semitrans);

    setXYWH(poly, rect->x + 0x80, rect->y, rect->w, rect->h);
    setUVWH(poly, u, v, rect->w, rect->h);
    poly->clut = clut;
    poly->tpage = texture_page;

    addPrim(&g_bunit_gfx_context->otag[otag_index], poly);
}
