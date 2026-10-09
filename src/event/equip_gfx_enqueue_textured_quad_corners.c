#include "fft/event_equip.h"
#include "psx/types.h"

void equip_gfx_enqueue_textured_quad_corners(
    const world_gfx_textured_quad_source_t* descriptor, const u8* color, s32 semitrans, s32 otag_index) {
    s32 quad_semitrans = semitrans;
    s32 quad_otag = otag_index;
    POLY_FT4* poly;

    poly = &g_equip_gfx_context->textured_quads[g_equip_gfx_poly_ft4_count++];

    if (color != 0) {
        SetShadeTex(poly, 0);
        poly->r0 = color[0];
        poly->g0 = color[1];
        poly->b0 = color[2];
    } else {
        SetShadeTex(poly, 1);
    }
    SetSemiTrans(poly, quad_semitrans);

    poly->x0 = descriptor->x0 + 0x80;
    poly->y0 = descriptor->y0;
    poly->x1 = descriptor->x1 + 0x80;
    poly->y1 = descriptor->y1;
    poly->x2 = descriptor->x2 + 0x80;
    poly->y2 = descriptor->y2;
    poly->x3 = descriptor->x3 + 0x80;
    poly->y3 = descriptor->y3;

    poly->u0 = descriptor->u0;
    poly->v0 = descriptor->v0;
    poly->u1 = descriptor->u1;
    poly->v1 = descriptor->v1;
    poly->u2 = descriptor->u2;
    poly->v2 = descriptor->v2;
    poly->u3 = descriptor->u3;
    poly->v3 = descriptor->v3;
    poly->clut = descriptor->clut;
    poly->tpage = descriptor->tpage;

    addPrim(&g_equip_gfx_context->otag[quad_otag], poly);
}
