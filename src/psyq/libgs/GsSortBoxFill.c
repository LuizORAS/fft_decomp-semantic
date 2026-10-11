#include "psx/libgpu.h"
#include "psx/libgs.h"
#include "psx/types.h"

/* The packet libgs sorts for a GsBOXF: a DR_TPAGE-style tag and draw-mode
 * word followed by the TILE body under the same tag. */
typedef struct {
    DR_TPAGE tpage; /* 0x00 */
    u8 r0;          /* 0x08 */
    u8 g0;          /* 0x09 */
    u8 b0;          /* 0x0A */
    u8 code;        /* 0x0B */
    s16 x0;         /* 0x0C */
    s16 y0;         /* 0x0E */
    u16 w;          /* 0x10 */
    u16 h;          /* 0x12 */
} psyq_gs_boxf_packet_t;

void GsSortBoxFill(GsBOXF* bp, GsOT* otp, u16 pri) {
    psyq_gs_boxf_packet_t* packet;
    s32 attr; /* Attribute packing uses signed right shifts. */
    u32 hi;
    u32 lo;

    attr = bp->attribute;
    if (!(attr & GsDOFF)) {
        packet = (psyq_gs_boxf_packet_t*)GsOUT_PACKET_P;
        hi = (attr >> 17) & 0x180;
        lo = ((attr >> 23) & 0x60) | 0xE1000200;
        packet->tpage.code[0] = hi | lo;
        setRGB0(packet, bp->r, bp->g, bp->b);
        packet->code = ((attr >> 29) & 2) | 0x60;
        setXY0(packet, bp->x + g_psyq_gs_offset_x, bp->y + g_psyq_gs_offset_y);
        setWH(packet, bp->w, bp->h);
        GsOUT_PACKET_P = psyq_gs_sort_sprite_bg((u32*)packet, otp, pri, 4);
    }
}
