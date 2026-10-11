#include "fft/battle.h"
#include "psx/libc.h"
#include "psx/libgpu.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Queues one effect slot's billboard sprite: projects the slot origin for its
 * ordering-table depth, builds the rotated, camera-scaled 2D matrix from the
 * slot angle, transforms the frame's four corners into the slot's
 * double-buffered POLY_FT4, and adds that quad between the slot's two draw-mode
 * primitives. */
void battle_effect_draw_billboard_sprite(battle_effect_sprite_slot_t* slot) {
    SVECTOR local;
    VECTOR position;
    s32 sxy;
    s32 flag;
    battle_effect_quad_prim_t* prim;
    battle_effect_sprite_frame_t* frame;
    u16 offset;
    u8* frame_group;
    s32 depth;
    s32 sine;
    s32 cosine;
    s32 zoom;
    s32 scale_y;
    u8 corner_u;
    u8 corner_v;
    u8 right;
    u8 bottom;

    SetRotMatrix(&g_battle_camera_matrix);
    SetTransMatrix(&g_battle_camera_matrix);
    setVector(&local, slot->x >> 12, slot->y >> 12, slot->z >> 12);
    RotTrans(&local, &position, &position.pad);
    depth = (position.vz >> 2) - 8;
    if (depth < EFFECT_OT_DEPTH_MIN) {
        depth = EFFECT_OT_DEPTH_MIN;
    }
    if (depth >= EFFECT_OT_DEPTH_LIMIT) {
        depth = EFFECT_OT_DEPTH_MAX;
    }
    g_battle_effect_matrix.t[0] = position.vx;
    g_battle_effect_matrix.t[1] = position.vy;
    cosine = rcos(slot->angle);
    sine = rsin(slot->angle);
    zoom = g_battle_camera_zoom.vx;
    scale_y = g_battle_camera_zoom.vy;
    g_battle_effect_matrix.m[0][0] = (cosine * zoom) >> 12;
    g_battle_effect_matrix.m[1][1] = (cosine * scale_y) >> 12;
    g_battle_effect_matrix.m[0][1] = (-sine * scale_y) >> 12;
    g_battle_effect_matrix.m[1][0] = (sine * zoom) >> 12;
    SetRotMatrix(&g_battle_effect_matrix);
    SetTransMatrix(&g_battle_effect_matrix);

    frame_group = (u8*)g_battle_effect_trap_frame_data.groups;
    offset
        = *(u16*)(frame_group + (slot->color.frame * 2 + g_battle_effect_trap_frame_data.groups[0].frame_table_offset));
    prim = &slot->quads[g_battle_effect_buffer_index];
    memcpy(&prim->color, &slot->color, sizeof(prim->color));
    setPolyFT4(prim);
    frame = (battle_effect_sprite_frame_t*)(frame_group + offset);
    setSemiTrans(prim, frame->flags & EFFECT_SPRITE_PART_FLAG_SEMI_TRANSPARENT);
    /* Copy x/y while retaining the GTE input's z. */
    memcpy(&g_battle_effect_corner_top_left, &frame->corners[0], sizeof(frame->corners[0]));
    memcpy(&g_battle_effect_corner_top_right, &frame->corners[1], sizeof(frame->corners[1]));
    memcpy(&g_battle_effect_corner_bottom_left, &frame->corners[2], sizeof(frame->corners[2]));
    memcpy(&g_battle_effect_corner_bottom_right, &frame->corners[3], sizeof(frame->corners[3]));
    RotTransPers3(&g_battle_effect_corner_top_left, &g_battle_effect_corner_top_right,
        &g_battle_effect_corner_bottom_left, &prim->xy0, &prim->xy1, &prim->xy2, &sxy, &flag);
    RotTransPers(&g_battle_effect_corner_bottom_right, &prim->xy3, &sxy, &flag);
    prim->tpage = (frame->flags & EFFECT_SPRITE_PART_TPAGE_MODE_MASK) | 8;
    prim->clut = slot->clut;
    corner_u = frame->u;
    prim->u2 = corner_u;
    prim->u0 = corner_u;
    right = corner_u + frame->width;
    prim->u3 = right;
    prim->u1 = right;
    corner_v = frame->v;
    prim->v1 = corner_v;
    prim->v0 = corner_v;
    bottom = corner_v + frame->height;
    prim->v3 = bottom;
    prim->v2 = bottom;
    SetDrawMode(&slot->modes[g_battle_effect_buffer_index][0], 1, 1, 1, 0);
    AddPrim(&g_battle_effect_polygon_depth_data[depth], &slot->modes[g_battle_effect_buffer_index][0]);
    AddPrim(&g_battle_effect_polygon_depth_data[depth], prim);
    SetDrawMode(&slot->modes[g_battle_effect_buffer_index][1], 1, 0, 1, 0);
    AddPrim(&g_battle_effect_polygon_depth_data[depth], &slot->modes[g_battle_effect_buffer_index][1]);
}
