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
    setXY4(poly, source->x + 0x80, source->y, (source->x + 0x80) + source->w, source->y, source->x + 0x80,
        source->y + source->h, (source->x + 0x80) + source->w, source->y + source->h);
    setUV4(poly, source->u, source->v, source->u + source->w, source->v, source->u, source->v + source->h,
        source->u + source->w, source->v + source->h);
    setRGB0(poly, colors[0].r, colors[0].g, colors[0].b);
    setRGB1(poly, colors[1].r, colors[1].g, colors[1].b);
    setRGB2(poly, colors[2].r, colors[2].g, colors[2].b);
    setRGB3(poly, colors[3].r, colors[3].g, colors[3].b);
    poly->clut = source->clut;
    poly->tpage = source->tpage;
    addPrim(&g_world_gfx_active_packet_buffer->otag[ot_index], poly);
}
