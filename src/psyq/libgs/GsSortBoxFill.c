#include "psx/libgpu.h"
#include "psx/libgs.h"
#include "psx/types.h"

void GsSortBoxFill(GsBOXF* bp, GsOT* otp, u16 pri) {
    TILE* packet;
    DR_TPAGE* draw_mode;
    s32 attr; /* Attribute packing uses signed right shifts. */
    u32 hi;
    u32 lo;

    attr = bp->attribute;
    if (!(attr & GsDOFF)) {
        draw_mode = (DR_TPAGE*)GsOUT_PACKET_P;
        /* Share one DMA tag: the mode word occupies this primitive view's tag slot. */
        packet = (TILE*)draw_mode->code;
        hi = (attr >> 17) & 0x180;
        lo = ((attr >> 23) & 0x60) | 0xE1000200;
        draw_mode->code[0] = hi | lo;
        setRGB0(packet, bp->r, bp->g, bp->b);
        packet->code = ((attr >> 29) & 2) | 0x60;
        setXY0(packet, bp->x + GsORGOFSX, bp->y + GsORGOFSY);
        setWH(packet, bp->w, bp->h);
        GsOUT_PACKET_P = psyq_gs_sort_sprite_bg((u32*)draw_mode, otp, pri, 4);
    }
}
