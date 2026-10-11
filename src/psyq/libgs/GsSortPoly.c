#include "psx/libgs.h"
#include "psx/stddef.h"

/* Copy a polygon to the work area, apply the 2D offset and link it into the OT.
 * Code bits 0x04/0x08/0x10 select texture words, a fourth vertex and vertex colors. */
void GsSortPoly(void* prim, GsOT* ot, u16 pri) {
    u32* src;
    u32* dst;
    DVECTOR* source_point;
    DVECTOR* destination_point;
    s32 len;
    s32 code;
    s32 gouraud;
    s32 quad;
    s32 textured;

    src = prim;
    dst = GsOUT_PACKET_P;
    len = 4;
    code = getcode(src);
    gouraud = code & 0x10;
    quad = code & 0x8;
    textured = code & 0x4;
    /* POLY_F4 supplies the bare corner positions; optional words shift these bases.
     * A fully advancing cursor changes the original constant coordinate offsets. */
    dst[offsetof(POLY_F4, r0) / sizeof(*dst)] = src[offsetof(POLY_F4, r0) / sizeof(*src)];
    source_point = (DVECTOR*)&src[offsetof(POLY_F4, x0) / sizeof(u32)];
    destination_point = (DVECTOR*)&dst[offsetof(POLY_F4, x0) / sizeof(u32)];
    destination_point->vx = source_point->vx + g_psyq_gs_offset_x;
    destination_point->vy = source_point->vy + g_psyq_gs_offset_y;
    if (textured) {
        dst[offsetof(POLY_F4, x1) / sizeof(u32)] = src[offsetof(POLY_F4, x1) / sizeof(u32)];
        src++;
        dst++;
        len++;
    }
    if (gouraud) {
        dst[offsetof(POLY_F4, x1) / sizeof(u32)] = src[offsetof(POLY_F4, x1) / sizeof(u32)];
        src++;
        dst++;
        len++;
    }
    source_point = (DVECTOR*)&src[offsetof(POLY_F4, x1) / sizeof(u32)];
    destination_point = (DVECTOR*)&dst[offsetof(POLY_F4, x1) / sizeof(u32)];
    destination_point->vx = source_point->vx + g_psyq_gs_offset_x;
    destination_point->vy = source_point->vy + g_psyq_gs_offset_y;
    if (textured) {
        dst[offsetof(POLY_F4, x2) / sizeof(u32)] = src[offsetof(POLY_F4, x2) / sizeof(u32)];
        src++;
        dst++;
        len++;
    }
    if (gouraud) {
        dst[offsetof(POLY_F4, x2) / sizeof(u32)] = src[offsetof(POLY_F4, x2) / sizeof(u32)];
        src++;
        dst++;
        len++;
    }
    source_point = (DVECTOR*)&src[offsetof(POLY_F4, x2) / sizeof(u32)];
    destination_point = (DVECTOR*)&dst[offsetof(POLY_F4, x2) / sizeof(u32)];
    destination_point->vx = source_point->vx + g_psyq_gs_offset_x;
    destination_point->vy = source_point->vy + g_psyq_gs_offset_y;
    if (textured) {
        dst[offsetof(POLY_F4, x3) / sizeof(u32)] = src[offsetof(POLY_F4, x3) / sizeof(u32)];
        src++;
        dst++;
        len++;
    }
    if (quad) {
        if (gouraud) {
            dst[offsetof(POLY_F4, x3) / sizeof(u32)] = src[offsetof(POLY_F4, x3) / sizeof(u32)];
            src++;
            dst++;
            len++;
        }
        source_point = (DVECTOR*)&src[offsetof(POLY_F4, x3) / sizeof(u32)];
        destination_point = (DVECTOR*)&dst[offsetof(POLY_F4, x3) / sizeof(u32)];
        destination_point->vx = source_point->vx + g_psyq_gs_offset_x;
        destination_point->vy = source_point->vy + g_psyq_gs_offset_y;
        len++;
        if (textured) {
            dst[sizeof(POLY_F4) / sizeof(*dst)] = src[sizeof(POLY_F4) / sizeof(*src)];
            len++;
        }
    }
    GsOUT_PACKET_P = psyq_gs_sort_sprite_bg(GsOUT_PACKET_P, ot, pri, len);
}
