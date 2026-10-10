#include "psx/libgs.h"

/* A polygon word holds coordinates or the texture/color data before them. */
typedef union {
    struct {
        s16 x;
        s16 y;
    } xy;
    u32 word;
} psyq_gs_poly_word_t;

/* A sliding view of the polygon body. Each optional texture/color word
 * advances the view so the remaining coordinates retain their named offsets. */
typedef struct {
    u32 tag; /* 0x00 */
    union {
        u32 word;
        struct {
            u8 r;
            u8 g;
            u8 b;
            u8 code;
        } bytes;
    } color;                     /* 0x04 */
    psyq_gs_poly_word_t vertex0; /* 0x08 */
    psyq_gs_poly_word_t vertex1; /* 0x0c */
    psyq_gs_poly_word_t vertex2; /* 0x10 */
    psyq_gs_poly_word_t vertex3; /* 0x14 */
    u32 texture3;                /* 0x18 */
} psyq_gs_poly_packet_t;

/* Copy a polygon to the work area, apply the 2D offset and link it into the OT.
 * Code bits 0x04/0x08/0x10 select texture words, a fourth vertex and vertex colors. */
void GsSortPoly(void* prim, GsOT* ot, u16 pri) {
    psyq_gs_poly_packet_t* src;
    psyq_gs_poly_packet_t* dst;
    s32 len;
    s32 code;
    s32 gouraud;
    s32 quad;
    s32 textured;

    src = prim;
    dst = GsOUT_PACKET_P;
    len = 4;
    code = src->color.bytes.code;
    gouraud = code & 0x10;
    quad = code & 0x8;
    textured = code & 0x4;
    dst->color.word = src->color.word;
    dst->vertex0.xy.x = src->vertex0.xy.x + g_psyq_gs_offset_x;
    dst->vertex0.xy.y = src->vertex0.xy.y + g_psyq_gs_offset_y;
    if (textured) {
        dst->vertex1.word = src->vertex1.word;
        src = (psyq_gs_poly_packet_t*)&src->color;
        dst = (psyq_gs_poly_packet_t*)&dst->color;
        len++;
    }
    if (gouraud) {
        dst->vertex1.word = src->vertex1.word;
        src = (psyq_gs_poly_packet_t*)&src->color;
        dst = (psyq_gs_poly_packet_t*)&dst->color;
        len++;
    }
    dst->vertex1.xy.x = src->vertex1.xy.x + g_psyq_gs_offset_x;
    dst->vertex1.xy.y = src->vertex1.xy.y + g_psyq_gs_offset_y;
    if (textured) {
        dst->vertex2.word = src->vertex2.word;
        src = (psyq_gs_poly_packet_t*)&src->color;
        dst = (psyq_gs_poly_packet_t*)&dst->color;
        len++;
    }
    if (gouraud) {
        dst->vertex2.word = src->vertex2.word;
        src = (psyq_gs_poly_packet_t*)&src->color;
        dst = (psyq_gs_poly_packet_t*)&dst->color;
        len++;
    }
    dst->vertex2.xy.x = src->vertex2.xy.x + g_psyq_gs_offset_x;
    dst->vertex2.xy.y = src->vertex2.xy.y + g_psyq_gs_offset_y;
    if (textured) {
        dst->vertex3.word = src->vertex3.word;
        src = (psyq_gs_poly_packet_t*)&src->color;
        dst = (psyq_gs_poly_packet_t*)&dst->color;
        len++;
    }
    if (quad) {
        if (gouraud) {
            dst->vertex3.word = src->vertex3.word;
            src = (psyq_gs_poly_packet_t*)&src->color;
            dst = (psyq_gs_poly_packet_t*)&dst->color;
            len++;
        }
        dst->vertex3.xy.x = src->vertex3.xy.x + g_psyq_gs_offset_x;
        dst->vertex3.xy.y = src->vertex3.xy.y + g_psyq_gs_offset_y;
        len++;
        if (textured) {
            dst->texture3 = src->texture3;
            len++;
        }
    }
    GsOUT_PACKET_P = psyq_gs_sort_sprite_bg(GsOUT_PACKET_P, ot, pri, len);
}
