#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Takes one POLY_FT4 from the pool per sprite cell, fills it as a w x h quad
 * at (x + 0x80, y) with texture origin (u, v), and links it into ordering
 * table entry ot_index. With rgb the quads are shaded with that colour. */
void world_gfx_enqueue_textured_quad_list(
    const menu_sprite_cell_t* cell, const rgb24_t* rgb, s32 semi_trans, u16 tpage, u16 clut, s32 ot_index, s32 count) {
    POLY_FT4* poly;

    count--;
    while (count >= 0) {
        poly = &g_world_gfx_active_packet_buffer->textured_quads[g_world_gfx_textured_quad_count++];
        if (rgb != 0) {
            SetShadeTex(poly, 0);
            setRGB0(poly, rgb->r, rgb->g, rgb->b);
        } else {
            SetShadeTex(poly, 1);
        }
        SetSemiTrans(poly, semi_trans);
        setXY4(poly, cell->x + 0x80, cell->y, cell->w + (cell->x + 0x80), cell->y, cell->x + 0x80, cell->y + cell->h,
            cell->w + (cell->x + 0x80), cell->y + cell->h);
        setUV4(poly, cell->u, cell->v, cell->u + cell->w, cell->v, cell->u, cell->v + cell->h, cell->u + cell->w,
            cell->v + cell->h);
        poly->clut = clut;
        poly->tpage = tpage;
        count--;
        cell++;
        addPrim(&g_world_gfx_active_packet_buffer->otag[ot_index], poly);
    }
}
