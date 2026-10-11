#include "fft/world.h"
#include "psx/libgpu.h"

/* Append a two-color line to the menu ordering table. */
void world_menu_add_gradient_line_primitive(point16_t* endpoints, rgb24_t* colors, s32 semi_trans, s32 ot_index) {
    LINE_G2* line = &g_world_gfx_active_packet_buffer->gradient_lines[g_world_gfx_gradient_line_count++];
    SetSemiTrans(line, semi_trans);
    line->x0 = endpoints[0].x + 0x80;
    line->y0 = endpoints[0].y;
    line->x1 = endpoints[1].x + 0x80;
    line->y1 = endpoints[1].y;
    line->r0 = colors[0].r;
    line->g0 = colors[0].g;
    line->b0 = colors[0].b;
    line->r1 = colors[1].r;
    line->g1 = colors[1].g;
    line->b1 = colors[1].b;
    addPrim(&g_world_gfx_active_packet_buffer->otag[ot_index], line);
}
