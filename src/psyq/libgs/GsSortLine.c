#include "psx/libgpu.h"
#include "psx/libgs.h"
#include "psx/types.h"

/* Build and link the flat-shaded line packet. */
void GsSortLine(GsLINE* line, GsOT* ot, u16 z) {
    LINE_F2* prim;
    DR_TPAGE* draw_mode;
    s32 attr; /* Attribute packing uses signed right shifts. */

    attr = line->attribute;
    if (!(attr & GsDOFF)) {
        draw_mode = (DR_TPAGE*)GsOUT_PACKET_P;
        /* Share one DMA tag: the mode word occupies this primitive view's tag slot. */
        prim = (LINE_F2*)draw_mode->code;
        draw_mode->code[0] = ((attr >> 23) & 0x60) | 0xE1000200;
        setRGB0(prim, line->r, line->g, line->b);
        prim->code = ((attr >> 29) & 2) | 0x40;
        setXY2(prim, line->x0 + g_psyq_gs_offset_x, line->y0 + g_psyq_gs_offset_y, line->x1 + g_psyq_gs_offset_x,
            line->y1 + g_psyq_gs_offset_y);
        GsOUT_PACKET_P = psyq_gs_sort_sprite_bg((u32*)draw_mode, ot, z, 4);
    }
}
