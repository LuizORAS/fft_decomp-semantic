#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Takes the next LINE_F2 packet from the pool (its SetLineF2 code is kept),
 * draws from (rect x, y) to (rect w, h) with an x bias of 0x80, and links it
 * into ordering-table entry ot_index (libgpu addPrim). */
void world_menu_add_flat_line_primitive(RECT* rect, u8* rgb, u8 semi_trans, s32 ot_index) {
    LINE_F2* line = &g_world_gfx_active_packet_buffer->flat_lines[g_world_gfx_flat_line_count++];

    line->r0 = rgb[0];
    line->g0 = rgb[1];
    line->b0 = rgb[2];
    SetSemiTrans(line, semi_trans);
    line->x0 = rect->x + 0x80;
    line->y0 = rect->y;
    line->x1 = rect->w + 0x80;
    line->y1 = rect->h;
    setaddr(line, getaddr(&g_world_gfx_active_packet_buffer->otag[ot_index]));
    setaddr(&g_world_gfx_active_packet_buffer->otag[ot_index], line);
}
