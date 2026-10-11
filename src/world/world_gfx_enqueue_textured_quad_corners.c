#include "fft/world.h"
#include "psx/libgpu.h"

/*
 * Append a textured menu quad to the selected ordering-table entry.
 *
 * Screen x coordinates use the menu's +0x80 origin. With `rgb` the quad is
 * shaded with that color; otherwise texture shading is disabled.
 */
void world_gfx_enqueue_textured_quad_corners(
    world_gfx_textured_quad_source_t* source, rgb24_t* rgb, s32 semi_trans, s32 ot_index) {
    POLY_FT4* poly = &g_world_gfx_active_packet_buffer->textured_quads[g_world_gfx_textured_quad_count++];
    if (rgb != 0) {
        SetShadeTex(poly, 0);
        setRGB0(poly, rgb->r, rgb->g, rgb->b);
    } else {
        SetShadeTex(poly, 1);
    }
    SetSemiTrans(poly, semi_trans);
    setXY4(poly, source->x0 + 0x80, source->y0, source->x1 + 0x80, source->y1, source->x2 + 0x80, source->y2,
        source->x3 + 0x80, source->y3);
    setUV4(poly, source->u0, source->v0, source->u1, source->v1, source->u2, source->v2, source->u3, source->v3);
    poly->clut = source->clut;
    poly->tpage = source->tpage;
    addPrim(&g_world_gfx_active_packet_buffer->otag[ot_index], poly);
}
