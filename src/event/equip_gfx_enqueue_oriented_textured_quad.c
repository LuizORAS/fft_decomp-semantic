#include "fft/event_equip.h"
#include "psx/types.h"

void equip_gfx_enqueue_oriented_textured_quad(
    const world_oriented_quad_t* descriptor, const u8* color, s32 orientation, s32 semitrans, s32 otag_index) {
    s32 quad_semitrans = semitrans;
    s32 quad_orientation = orientation;
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

    poly->x0 = descriptor->x + 0x80;
    poly->y0 = descriptor->y;
    poly->x1 = descriptor->x + (descriptor->w + 0x80);
    poly->y1 = descriptor->y;
    poly->x2 = descriptor->x + 0x80;
    poly->y2 = descriptor->y + descriptor->h;
    poly->x3 = descriptor->x + (descriptor->w + 0x80);
    poly->y3 = descriptor->y + descriptor->h;

    switch (quad_orientation & 0xff) {
    case TEXTURE_UV_FLIP_NONE:
        poly->u0 = descriptor->u;
        poly->v0 = descriptor->v;
        poly->u1 = descriptor->u + descriptor->uw;
        poly->v1 = descriptor->v;
        poly->u2 = descriptor->u;
        poly->v2 = descriptor->v + descriptor->vh;
        poly->u3 = descriptor->u + descriptor->uw;
        poly->v3 = descriptor->v + descriptor->vh;
        break;
    case TEXTURE_UV_FLIP_X:
        poly->u1 = descriptor->u;
        poly->v1 = descriptor->v;
        poly->u0 = descriptor->u + descriptor->uw;
        poly->v0 = descriptor->v;
        poly->u3 = descriptor->u;
        poly->v3 = descriptor->v + descriptor->vh;
        poly->u2 = descriptor->u + descriptor->uw;
        poly->v2 = descriptor->v + descriptor->vh;
        break;
    case TEXTURE_UV_FLIP_Y:
        poly->u2 = descriptor->u;
        poly->v2 = descriptor->v;
        poly->u3 = descriptor->u + descriptor->uw;
        poly->v3 = descriptor->v;
        poly->u0 = descriptor->u;
        poly->v0 = descriptor->v + descriptor->vh;
        poly->u1 = descriptor->u + descriptor->uw;
        poly->v1 = descriptor->v + descriptor->vh;
        break;
    case TEXTURE_UV_FLIP_XY:
        poly->u3 = descriptor->u;
        poly->v3 = descriptor->v;
        poly->u2 = descriptor->u + descriptor->uw;
        poly->v2 = descriptor->v;
        poly->u1 = descriptor->u;
        poly->v1 = descriptor->v + descriptor->vh;
        poly->u0 = descriptor->u + descriptor->uw;
        poly->v0 = descriptor->v + descriptor->vh;
        break;
    }

    poly->clut = descriptor->clut;
    poly->tpage = descriptor->tpage;
    addPrim(&g_equip_gfx_context->otag[otag_index], poly);
}
