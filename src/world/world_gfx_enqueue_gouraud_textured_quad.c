#include "fft/world.h"
#include "psx/libgpu.h"

/*
 * Append a Gouraud-shaded textured rectangle to the selected ordering-table
 * entry. Screen x coordinates use the menu's +0x80 origin; `rgb` holds four
 * RGB triples, one per corner.
 */
void world_gfx_enqueue_gouraud_textured_quad(
    world_gfx_textured_rect_source_t* source, rgb24_t* colors, s32 semi_trans, s32 ot_index) {
    POLY_GT4* poly
        = &g_world_gfx_active_packet_buffer->textured_gradient_quads[g_world_gfx_textured_gradient_quad_count++];

    SetSemiTrans(poly, semi_trans);
    poly->x0 = source->x + 0x80;
    poly->y0 = source->y;
    poly->x1 = (source->x + 0x80) + source->w;
    poly->y1 = source->y;
    poly->x2 = source->x + 0x80;
    poly->y2 = source->y + source->h;
    poly->x3 = (source->x + 0x80) + source->w;
    poly->y3 = source->y + source->h;
    poly->u0 = source->u;
    poly->v0 = source->v;
    poly->u1 = source->u + source->w;
    poly->v1 = source->v;
    poly->u2 = source->u;
    poly->v2 = source->v + source->h;
    poly->u3 = source->u + source->w;
    poly->v3 = source->v + source->h;
    poly->r0 = colors[0].r;
    poly->g0 = colors[0].g;
    poly->b0 = colors[0].b;
    poly->r1 = colors[1].r;
    poly->g1 = colors[1].g;
    poly->b1 = colors[1].b;
    poly->r2 = colors[2].r;
    poly->g2 = colors[2].g;
    poly->b2 = colors[2].b;
    poly->r3 = colors[3].r;
    poly->g3 = colors[3].g;
    poly->b3 = colors[3].b;
    poly->clut = source->clut;
    poly->tpage = source->tpage;
    addPrim(&g_world_gfx_active_packet_buffer->otag[ot_index], poly);
}
