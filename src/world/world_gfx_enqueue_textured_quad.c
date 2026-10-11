#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Takes one POLY_FT4 from the pool, fills it as the rect->w x rect->h quad at
 * (rect->x + 0x80, rect->y) with texture origin (u, v), and links it into
 * ordering table entry priority. With color the quad is shaded with it. */
void world_gfx_enqueue_textured_quad(
    RECT* rect, s32 u, s32 v, rgb24_t* color, s32 semi_trans, u16 texture_page, u16 clut, s32 priority) {
    POLY_FT4* poly;
    rgb24_t* rgb = color;

    poly = &g_world_gfx_active_packet_buffer->textured_quads[g_world_gfx_textured_quad_count++];
    if (rgb != 0) {
        SetShadeTex(poly, 0);
        setRGB0(poly, rgb->r, rgb->g, rgb->b);
    } else {
        SetShadeTex(poly, 1);
    }
    SetSemiTrans(poly, semi_trans);
    setXY4(poly, rect->x + 0x80, rect->y, rect->x + 0x80 + rect->w, rect->y, rect->x + 0x80, rect->y + rect->h,
        rect->x + 0x80 + rect->w, rect->y + rect->h);
    setUV4(poly, u, v, u + rect->w, v, u, v + rect->h, u + rect->w, v + rect->h);
    poly->clut = clut;
    poly->tpage = texture_page;
    addPrim(&g_world_gfx_active_packet_buffer->otag[priority], poly);
}
