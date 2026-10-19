#include "psx/libgs.h"

/* Copies one GPU polygon primitive into the libgs packet area, offsetting
 * each vertex by GsORGOFSX and GsORGOFSY, and sorts it into the OT at `pri`.
 * The 0x04/0x08/0x10 code bits add per-vertex texture words, a fourth vertex
 * and per-vertex colour words. */
void GsSortPoly(void* prim, GsOT* ot, u16 pri) {
    u32* src;
    u32* dst;
    POLY_F4* source_coordinates;
    POLY_F4* destination_coordinates;
    s32 len;
    u8 code;
    s32 gouraud;
    s32 quad;
    s32 textured;

    src = prim;
    dst = (u32*)GsOUT_PACKET_P;
    len = PSYQ_GPU_PACKET_WORDS(POLY_F3);
    code = getcode(src);
    /* Byte or bitfield locals add normalization to these integer masks. */
    gouraud = code & PSYQ_GPU_POLYGON_GOURAUD;
    quad = code & PSYQ_GPU_POLYGON_QUAD;
    textured = code & PSYQ_GPU_POLYGON_TEXTURED;
    /* The cursor skips 32-bit UV/color fields; POLY_F4 supplies only the coordinate layout.
     * Fully advancing coordinate pointers changes the original load/store offsets. */
    dst[offsetof(P_TAG, r0) / sizeof(*dst)] = src[offsetof(P_TAG, r0) / sizeof(*src)];
    source_coordinates = (POLY_F4*)src;
    destination_coordinates = (POLY_F4*)dst;
    destination_coordinates->x0 = source_coordinates->x0 + GsORGOFSX;
    destination_coordinates->y0 = source_coordinates->y0 + GsORGOFSY;
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
    source_coordinates = (POLY_F4*)src;
    destination_coordinates = (POLY_F4*)dst;
    destination_coordinates->x1 = source_coordinates->x1 + GsORGOFSX;
    destination_coordinates->y1 = source_coordinates->y1 + GsORGOFSY;
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
    source_coordinates = (POLY_F4*)src;
    destination_coordinates = (POLY_F4*)dst;
    destination_coordinates->x2 = source_coordinates->x2 + GsORGOFSX;
    destination_coordinates->y2 = source_coordinates->y2 + GsORGOFSY;
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
        source_coordinates = (POLY_F4*)src;
        destination_coordinates = (POLY_F4*)dst;
        destination_coordinates->x3 = source_coordinates->x3 + GsORGOFSX;
        destination_coordinates->y3 = source_coordinates->y3 + GsORGOFSY;
        len++;
        if (textured) {
            dst[sizeof(POLY_F4) / sizeof(*dst)] = src[sizeof(POLY_F4) / sizeof(*src)];
            len++;
        }
    }
    GsOUT_PACKET_P = psyq_gs_sort_sprite_bg((u32*)GsOUT_PACKET_P, ot, pri, len);
}
