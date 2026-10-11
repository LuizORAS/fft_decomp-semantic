#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Draw a flat line from (rect->x, rect->y) to (rect->w, rect->h), with
 * an x bias of 0x80, and link it into ordering-table entry ot_index. */
void world_gfx_enqueue_line_f2(RECT* rect, rgb24_t* rgb, u8 semi_trans, s32 ot_index) {
    LINE_F2* line = &g_world_gfx_active_packet_buffer->flat_lines[g_world_gfx_flat_line_count++];

    setRGB0(line, rgb->r, rgb->g, rgb->b);
    SetSemiTrans(line, semi_trans);
    line->x0 = rect->x + 0x80;
    line->y0 = rect->y;
    line->x1 = rect->w + 0x80;
    line->y1 = rect->h;
    addPrim(&g_world_gfx_active_packet_buffer->otag[ot_index], line);
}
