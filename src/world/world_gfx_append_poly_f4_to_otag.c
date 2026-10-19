#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Take the next POLY_F4 from the pool, set its semi-transparency and colour, copy the four x/y pairs
 * (x shifted by 0x80) and link it into ordering-table entry ot_index (libgpu addPrim). No retail code calls
 * it. */
void world_gfx_append_poly_f4_to_otag(point16_t* xy, rgb24_t* rgb, s32 semi_trans, s32 ot_index) {
    POLY_F4* poly = &g_menu_gfx_context->poly_f4[g_world_gfx_poly_f4_count++];

    SetSemiTrans(poly, semi_trans);
    setRGB0(poly, rgb->r, rgb->g, rgb->b);
    setXY4(poly, xy[0].x + 0x80, xy[0].y, xy[1].x + 0x80, xy[1].y, xy[2].x + 0x80, xy[2].y, xy[3].x + 0x80, xy[3].y);
    addPrim(&g_menu_gfx_context->otag[ot_index], poly);
}
