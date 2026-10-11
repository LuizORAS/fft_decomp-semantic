#include "psx/libgpu.h"
#include "psx/libgs.h"
#include "psx/types.h"

/* The packet libgs sorts for a GsLINE: one four-word tag covering a
 * texture-page/draw-mode word and the LINE_F2 body, so it opens like a
 * DR_TPAGE and continues with the line's colour, code and endpoints. */
typedef struct {
    DR_TPAGE tpage; /* 0x00 */
    u8 r0;          /* 0x08 */
    u8 g0;          /* 0x09 */
    u8 b0;          /* 0x0A */
    u8 code;        /* 0x0B */
    s16 x0;         /* 0x0C */
    s16 y0;         /* 0x0E */
    s16 x1;         /* 0x10 */
    s16 y1;         /* 0x12 */
} psyq_gs_line_packet_t;

/* Build and link the flat-shaded line packet. */
void GsSortLine(GsLINE* line, GsOT* ot, u16 z) {
    psyq_gs_line_packet_t* prim;
    s32 attr;

    attr = line->attribute;
    if (attr >= 0) {
        prim = (psyq_gs_line_packet_t*)GsOUT_PACKET_P;
        prim->tpage.code[0] = ((attr >> 23) & 0x60) | 0xE1000200;
        setRGB0(prim, line->r, line->g, line->b);
        prim->code = ((attr >> 29) & 2) | 0x40;
        setXY2(prim, line->x0 + g_psyq_gs_offset_x, line->y0 + g_psyq_gs_offset_y, line->x1 + g_psyq_gs_offset_x,
            line->y1 + g_psyq_gs_offset_y);
        GsOUT_PACKET_P = psyq_gs_sort_sprite_bg((u32*)prim, ot, z, 4);
    }
}
