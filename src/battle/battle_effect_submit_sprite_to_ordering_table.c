#include "fft/battle.h"
#include "psx/gte_inline_split.h"
#include "psx/libc.h"
#include "psx/libgpu.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Queues a multi-part effect sprite at a screen position: brackets the parts
 * with two draw-mode primitives, builds the 2D rotation (optionally scaled by
 * `zoom`) into the effect matrix, and projects each part's corners through the
 * GTE into a POLY_FT4 linked into `ot`. */
void battle_effect_submit_sprite_to_ordering_table(
    battle_effect_sprite_block_t* set, DVECTOR* position, s16 angle, VECTOR* zoom, u32* ot) {
    battle_effect_quad_prim_t* prim;
    battle_effect_quad_prim_t* quad;
    battle_effect_sprite_part_t* frame;
    s32 i;
    s16 cosine;
    s16 sine;
    s32 value;
    s32 clut;
    s32 code;

    prim = battle_effect_alloc_primitive(BATTLE_EFFECT_PRIMITIVE_DR_MODE);
    SetDrawMode((DR_MODE*)prim, 1, 1, 1, 0);
    addPrim(ot, prim);
    g_battle_effect_matrix.t[0] = position->vx;
    g_battle_effect_matrix.t[1] = position->vy;
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
    for (i = 0; i < set->color.sprite_count; i++) {
        frame = set->frames[i];
        code = 0x2c;
        /* Copy x/y while retaining the GTE input's z. */
        memcpy(&g_battle_effect_corner_top_left, &frame->corners[0], sizeof(frame->corners[0]));
        memcpy(&g_battle_effect_corner_top_right, &frame->corners[1], sizeof(frame->corners[1]));
        memcpy(&g_battle_effect_corner_bottom_left, &frame->corners[2], sizeof(frame->corners[2]));
        gte_ldv3_split(
            &g_battle_effect_corner_top_left, &g_battle_effect_corner_top_right, &g_battle_effect_corner_bottom_left);
        gte_rtpt_split();
        memcpy(&g_battle_effect_corner_bottom_right, &frame->corners[3], sizeof(frame->corners[3]));
        quad = battle_effect_alloc_primitive(BATTLE_EFFECT_PRIMITIVE_FT4);
        memcpy(&quad->color, &set->color, sizeof(quad->color));
        quad->length = 9;
        quad->color.cd = code;
        if (frame->flags & EFFECT_SPRITE_PART_FLAG_SEMI_TRANSPARENT) {
            quad->color.cd = 0x2e;
        }
        gte_stsxy3_split(&quad->xy0, &quad->xy1, &quad->xy2);
        gte_ldv0_split(&g_battle_effect_corner_bottom_right);
        gte_rtps_split();
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
        gte_stsxy_split(&quad->xy3);
        addPrim(ot, quad);
    }
    prim = battle_effect_alloc_primitive(BATTLE_EFFECT_PRIMITIVE_DR_MODE);
    SetDrawMode((DR_MODE*)prim, 1, 0, 1, 0);
    addPrim(ot, prim);
}
