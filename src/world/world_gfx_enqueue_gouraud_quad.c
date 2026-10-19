#include "fft/world.h"
#include "psx/libgpu.h"

/*
 * Append a four-color menu quad to the selected ordering-table entry.
 *
 * Screen x coordinates use the menu's +0x80 origin. Packet length and
 * opcode are initialized by the pool owner and retained when linking.
 */
void world_gfx_enqueue_gouraud_quad(point16_t* xy, rgb24_t* colors, s32 semi_trans, s32 ot_index) {
    POLY_G4* poly = &g_menu_gfx_context->poly_g4[g_world_gfx_gradient_quad_count++];
    SetSemiTrans(poly, semi_trans);
    setXY4(poly, xy[0].x + 0x80, xy[0].y, xy[1].x + 0x80, xy[1].y, xy[2].x + 0x80, xy[2].y, xy[3].x + 0x80, xy[3].y);
    setRGB0(poly, colors[0].r, colors[0].g, colors[0].b);
    setRGB1(poly, colors[1].r, colors[1].g, colors[1].b);
    setRGB2(poly, colors[2].r, colors[2].g, colors[2].b);
    setRGB3(poly, colors[3].r, colors[3].g, colors[3].b);
    addPrim(&g_menu_gfx_context->otag[ot_index], poly);
}
