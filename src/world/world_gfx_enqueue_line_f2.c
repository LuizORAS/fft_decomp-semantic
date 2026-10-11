#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Draw the two endpoints with an x bias of 0x80 and append the flat line. */
void world_gfx_enqueue_line_f2(point16_t endpoints[2], rgb24_t* rgb, u8 semi_trans, s32 ot_index) {
    LINE_F2* line = &g_world_gfx_active_packet_buffer->flat_lines[g_world_gfx_flat_line_count++];

    setRGB0(line, rgb->r, rgb->g, rgb->b);
    SetSemiTrans(line, semi_trans);
    line->x0 = endpoints[0].x + 0x80;
    line->y0 = endpoints[0].y;
    line->x1 = endpoints[1].x + 0x80;
    line->y1 = endpoints[1].y;
    addPrim(&g_world_gfx_active_packet_buffer->otag[ot_index], line);
}
