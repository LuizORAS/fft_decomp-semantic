#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/libgs.h"
#include "psx/types.h"

/* One word slot of a GPU polygon packet: a vertex position, or the texture
 * (uv/clut/tpage) or colour word that precedes the next vertex. */
typedef union {
    struct {
        s16 x;
        s16 y;
    } xy;
    u32 word;
} world_gs_poly_slot_t;

/* A polygon packet seen from its colour word; optional words shift the later
 * vertices, so the walker re-bases the view one word at a time. */
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
    } color;                       /* 0x04 */
    world_gs_poly_slot_t slots[5]; /* 0x08 */
} world_gs_poly_packet_t;

#define NEXT_WORD(p) ((world_gs_poly_packet_t*)((u32*)(p) + 1))

/* Copies one GPU polygon primitive into the libgs packet area, offsetting
 * each vertex by g_world_gs_offset_x/y, and sorts it into the OT at `pri`.
 * The 0x04/0x08/0x10 code bits add per-vertex texture words, a fourth vertex
 * and per-vertex colour words. */
void world_gs_sortpoly(void* prim, GsOT* ot, s32 pri) {
    world_gs_poly_packet_t* src;
    world_gs_poly_packet_t* dst;
    s32 len;
    s32 code;
    s32 gouraud;
    s32 quad;
    s32 textured;

    src = prim;
    dst = g_world_gs_out_packet_p;
    len = 4;
    code = src->color.bytes.code;
    gouraud = code & 0x10;
    quad = code & 0x8;
    textured = code & 0x4;
    dst->color.word = src->color.word;
    dst->slots[0].xy.x = src->slots[0].xy.x + g_world_gs_offset_x;
    dst->slots[0].xy.y = src->slots[0].xy.y + g_world_gs_offset_y;
    if (textured) {
        dst->slots[1].word = src->slots[1].word;
        src = NEXT_WORD(src);
        dst = NEXT_WORD(dst);
        len++;
    }
    if (gouraud) {
        dst->slots[1].word = src->slots[1].word;
        src = NEXT_WORD(src);
        dst = NEXT_WORD(dst);
        len++;
    }
    dst->slots[1].xy.x = src->slots[1].xy.x + g_world_gs_offset_x;
    dst->slots[1].xy.y = src->slots[1].xy.y + g_world_gs_offset_y;
    if (textured) {
        dst->slots[2].word = src->slots[2].word;
        src = NEXT_WORD(src);
        dst = NEXT_WORD(dst);
        len++;
    }
    if (gouraud) {
        dst->slots[2].word = src->slots[2].word;
        src = NEXT_WORD(src);
        dst = NEXT_WORD(dst);
        len++;
    }
    dst->slots[2].xy.x = src->slots[2].xy.x + g_world_gs_offset_x;
    dst->slots[2].xy.y = src->slots[2].xy.y + g_world_gs_offset_y;
    if (textured) {
        dst->slots[3].word = src->slots[3].word;
        src = NEXT_WORD(src);
        dst = NEXT_WORD(dst);
        len++;
    }
    if (quad) {
        if (gouraud) {
            dst->slots[3].word = src->slots[3].word;
            src = NEXT_WORD(src);
            dst = NEXT_WORD(dst);
            len++;
        }
        dst->slots[3].xy.x = src->slots[3].xy.x + g_world_gs_offset_x;
        dst->slots[3].xy.y = src->slots[3].xy.y + g_world_gs_offset_y;
        len++;
        if (textured) {
            dst->slots[4].word = src->slots[4].word;
            len++;
        }
    }
    g_world_gs_out_packet_p = (void*)world_ps_sort_sprite_bg(g_world_gs_out_packet_p, ot, (u16)pri, len);
}
