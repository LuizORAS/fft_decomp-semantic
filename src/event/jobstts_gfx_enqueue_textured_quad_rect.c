#include "fft/event_jobstts.h"

void jobstts_gfx_enqueue_textured_quad_rect(
    urect16_t* rect, world_item_icon_source_t* tex, u8* color, s32 semitrans, s32 ot_index) {
    POLY_FT4* poly;

    poly = &g_jobstts_gfx_context->textured_quads[g_jobstts_gfx_poly_ft4_count++];

    if (color != 0) {
        SetShadeTex(poly, 0);
        poly->r0 = color[0];
        poly->g0 = color[1];
        poly->b0 = color[2];
    } else {
        SetShadeTex(poly, 1);
    }
    SetSemiTrans(poly, semitrans);

    poly->x0 = rect->x + 0x80;
    poly->y0 = rect->y;
    poly->x1 = rect->x + (rect->w + 0x80);
    poly->y1 = rect->y;
    poly->x2 = rect->x + 0x80;
    poly->y2 = rect->y + rect->h;
    poly->x3 = rect->x + (rect->w + 0x80);
    poly->y3 = rect->y + rect->h;
    poly->u0 = tex->u;
    poly->v0 = tex->v;
    poly->u1 = tex->u + tex->w;
    poly->v1 = tex->v;
    poly->u2 = tex->u;
    poly->v2 = tex->v + tex->h;
    poly->u3 = tex->u + tex->w;
    poly->v3 = tex->v + tex->h;
    poly->clut = tex->clut;
    poly->tpage = tex->tpage;

    setaddr(poly, g_jobstts_gfx_context->otag[ot_index]);
    catPrim(&g_jobstts_gfx_context->otag[ot_index], poly);
}
