#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Takes the next POLY_F4 from the pool, sets semi-transparency, colours it,
 * copies the four x/y pairs (x shifted by 0x80) and links it into
 * ordering-table entry ot_index (libgpu addPrim). */
/* Target 0x8012bebc. */
void world_gfx_append_poly_f4_to_otag(point16_t* xy, rgb24_t* rgb, s32 semi_trans, s32 ot_index) {
    POLY_F4* poly = &g_world_gfx_active_packet_buffer->poly_f4s[g_world_gfx_poly_f4_count++];

    SetSemiTrans(poly, semi_trans);
    poly->r0 = rgb->r;
    poly->g0 = rgb->g;
    poly->b0 = rgb->b;
    poly->x0 = xy[0].x + 0x80;
    poly->y0 = xy[0].y;
    poly->x1 = xy[1].x + 0x80;
    poly->y1 = xy[1].y;
    poly->x2 = xy[2].x + 0x80;
    poly->y2 = xy[2].y;
    poly->x3 = xy[3].x + 0x80;
    poly->y3 = xy[3].y;
    addPrim(&g_world_gfx_active_packet_buffer->otag[ot_index], poly);
}
