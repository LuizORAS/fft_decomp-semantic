#include "fft/battle.h"
#include "psx/libc.h"
#include "psx/libgpu.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Psy-Q GTE macros in the form this routine was compiled with. The bytes are
 * the DMPSX sequences of include/psx/gte_inline.h, but each instruction is its
 * own asm statement: the three operand copies of ldv3/stsxy3 each reuse one
 * input register ($8, then $2), which one multi-operand statement cannot do,
 * and the per-instruction statement count sets the live lengths that make
 * global allocation give `zoom` $s3 ahead of `ot` ($s4). */
#define gte_ldv0(r0)                                                                                                   \
    {                                                                                                                  \
        __asm__ volatile("addu $12, %0, $0" : : "r"(r0) : "$12");                                                      \
        __asm__ volatile("lwc2 $0, 0($12)");                                                                           \
        __asm__ volatile("lwc2 $1, 4($12)");                                                                           \
    }

#define gte_ldv3(r0, r1, r2)                                                                                           \
    {                                                                                                                  \
        __asm__ volatile("addu $12, %0, $0" : : "r"(r0) : "$12");                                                      \
        __asm__ volatile("addu $13, %0, $0" : : "r"(r1) : "$13");                                                      \
        __asm__ volatile("addu $14, %0, $0" : : "r"(r2) : "$14");                                                      \
        __asm__ volatile("lwc2 $0, 0($12)");                                                                           \
        __asm__ volatile("lwc2 $1, 4($12)");                                                                           \
        __asm__ volatile("lwc2 $2, 0($13)");                                                                           \
        __asm__ volatile("lwc2 $3, 4($13)");                                                                           \
        __asm__ volatile("lwc2 $4, 0($14)");                                                                           \
        __asm__ volatile("lwc2 $5, 4($14)");                                                                           \
    }

#define gte_rtps()                                                                                                     \
    {                                                                                                                  \
        __asm__ volatile("nop");                                                                                       \
        __asm__ volatile("nop");                                                                                       \
        __asm__ volatile(".word 0x4a180001");                                                                          \
    }

#define gte_rtpt()                                                                                                     \
    {                                                                                                                  \
        __asm__ volatile("nop");                                                                                       \
        __asm__ volatile("nop");                                                                                       \
        __asm__ volatile(".word 0x4a280030");                                                                          \
    }

#define gte_stsxy(r0)                                                                                                  \
    {                                                                                                                  \
        __asm__ volatile("addu $12, %0, $0" : : "r"(r0) : "$12");                                                      \
        __asm__ volatile("swc2 $14, 0($12)" : : : "memory");                                                           \
    }

#define gte_stsxy3(r0, r1, r2)                                                                                         \
    {                                                                                                                  \
        __asm__ volatile("addu $12, %0, $0" : : "r"(r0) : "$12");                                                      \
        __asm__ volatile("addu $13, %0, $0" : : "r"(r1) : "$13");                                                      \
        __asm__ volatile("addu $14, %0, $0" : : "r"(r2) : "$14");                                                      \
        __asm__ volatile("swc2 $12, 0($12)" : : : "memory");                                                           \
        __asm__ volatile("swc2 $13, 0($13)" : : : "memory");                                                           \
        __asm__ volatile("swc2 $14, 0($14)" : : : "memory");                                                           \
    }

/* Queues a multi-part effect sprite at a screen position: brackets the parts
 * with two draw-mode primitives, builds the 2D rotation (optionally scaled by
 * `zoom`) into the effect matrix, and projects each part's corners through the
 * GTE into a POLY_FT4 linked into `ot`. */
void battle_effect_submit_sprite_to_ordering_table(
    battle_effect_sprite_block_t* set, s16* position, s16 angle, VECTOR* zoom, u32* ot) {
    battle_effect_quad_prim_t* prim;
    battle_effect_quad_prim_t* quad;
    battle_effect_sprite_part_t* frame;
    s32 i;
    s16 cosine;
    s16 sine;
    s32 value;
    s32 clut;
    u32 color;
    s32 code;

    prim = battle_effect_alloc_primitive(BATTLE_EFFECT_PRIMITIVE_DR_MODE);
    SetDrawMode((DR_MODE*)prim, 1, 1, 1, 0);
    addPrim(ot, prim);
    g_battle_effect_matrix.t[0] = position[0];
    g_battle_effect_matrix.t[1] = position[1];
    cosine = rcos(angle);
    sine = rsin(angle);
    if (zoom != 0) {
        g_battle_effect_matrix.m[0][0] = (cosine * zoom->vx) >> 12;
        g_battle_effect_matrix.m[1][1] = (cosine * zoom->vy) >> 12;
        g_battle_effect_matrix.m[0][1] = (-sine * zoom->vy) >> 12;
        g_battle_effect_matrix.m[1][0] = (sine * zoom->vx) >> 12;
    } else {
        g_battle_effect_matrix.m[1][0] = sine;
        g_battle_effect_matrix.m[1][1] = cosine;
        g_battle_effect_matrix.m[0][0] = cosine;
        g_battle_effect_matrix.m[0][1] = -sine;
    }
    SetRotMatrix(&g_battle_effect_matrix);
    SetTransMatrix(&g_battle_effect_matrix);
    for (i = 0; i < set->color.field.sprite_count; i++) {
        frame = set->frames[i];
        code = 0x2c;
        /* Copy x/y while retaining the GTE input's z. */
        memcpy(&g_battle_effect_corner_top_left, &frame->corners[0], sizeof(frame->corners[0]));
        memcpy(&g_battle_effect_corner_top_right, &frame->corners[1], sizeof(frame->corners[1]));
        memcpy(&g_battle_effect_corner_bottom_left, &frame->corners[2], sizeof(frame->corners[2]));
        gte_ldv3(
            &g_battle_effect_corner_top_left, &g_battle_effect_corner_top_right, &g_battle_effect_corner_bottom_left);
        gte_rtpt();
        memcpy(&g_battle_effect_corner_bottom_right, &frame->corners[3], sizeof(frame->corners[3]));
        quad = battle_effect_alloc_primitive(BATTLE_EFFECT_PRIMITIVE_FT4);
        color = set->color.packed;
        quad->length = 9;
        quad->color.packed = color;
        quad->color.field.cd = code;
        if (frame->flags & EFFECT_SPRITE_PART_FLAG_SEMI_TRANSPARENT) {
            quad->color.field.cd = 0x2e;
        }
        gte_stsxy3(&quad->xy0, &quad->xy1, &quad->xy2);
        gte_ldv0(&g_battle_effect_corner_bottom_right);
        gte_rtps();
        quad->tpage = frame->tpage;
        value = frame->flags & EFFECT_SPRITE_PART_CLUT_MASK;
        if (frame->flags & EFFECT_SPRITE_PART_FLAG_SECOND_CLUT_ROW) {
            clut = value + MAP_CLUT(MAP_CLUT_ROW_EFFECT_SECOND, 0);
        } else {
            clut = value + MAP_CLUT(MAP_CLUT_ROW_EFFECT, 0);
        }
        quad->clut = clut;
        quad->u0 = quad->u2 = frame->u;
        quad->u1 = quad->u3 = quad->u0 + frame->width;
        quad->v0 = quad->v1 = frame->v;
        quad->v2 = quad->v3 = quad->v0 + frame->height;
        gte_stsxy(&quad->xy3);
        addPrim(ot, quad);
    }
    prim = battle_effect_alloc_primitive(BATTLE_EFFECT_PRIMITIVE_DR_MODE);
    SetDrawMode((DR_MODE*)prim, 1, 0, 1, 0);
    addPrim(ot, prim);
}
