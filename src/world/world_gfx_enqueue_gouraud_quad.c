#include "fft/world.h"
#include "psx/libgpu.h"

/*
 * Append a four-color menu quad to the selected ordering-table entry.
 *
 * Screen x coordinates use the menu's +0x80 origin. Packet length and
 * opcode are initialized by the pool owner and retained when linking.
 */
void world_gfx_enqueue_gouraud_quad(point16_t* xy, rgb24_t* colors, s32 semi_trans, s32 ot_index) {
    POLY_G4* poly = &g_world_gfx_active_packet_buffer->gradient_quads[g_world_gfx_gradient_quad_count++];
    SetSemiTrans(poly, semi_trans);
    poly->x0 = xy[0].x + 0x80;
    poly->y0 = xy[0].y;
    poly->x1 = xy[1].x + 0x80;
    poly->y1 = xy[1].y;
    poly->x2 = xy[2].x + 0x80;
    poly->y2 = xy[2].y;
    poly->x3 = xy[3].x + 0x80;
    poly->y3 = xy[3].y;
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
    addPrim(&g_world_gfx_active_packet_buffer->otag[ot_index], poly);
}
