#include "fft/battle.h"
#include "psx/gte_inline.h"
#include "psx/libc.h"
#include "psx/libgpu.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Transformed model points live in the scratchpad as VECTORs, with the GTE
 * flag of each transform stored in `pad`. */
#define SCRATCH ((VECTOR*)0x1f800000)
/* Byte addressing preserves the original addition operand order. */
#define PT(k) (*(VECTOR*)((u8*)scratch + (command.vertices.indices[k] << 4)))

/* Draw one of the projectile models resident in BATTLE.BIN for the current
 * secondary effect: the low byte of parameter is a battle_effect_model_e.
 *
 * The model's vertices go through three transforms in the scratchpad: the spin
 * rotation, then the angles rotation with scale and position, then the camera
 * matrix. The model's draw commands are then decoded one by one into the effect's
 * work buffer for the current frame buffer and linked into the effect ordering
 * table:
 * - ARROW: G4 and G3 commands, at a quarter of the corners' average z (their sum
 *   over 16 or 12).
 * - STONE and SHURIKEN: G3 commands only, at the same depth.
 * - REFLECT: only the first timer * 3 commands, so the shield grows by three
 *   triangles a frame. Its GT3 commands use texture page 0x3f and the CLUT at
 *   0x7eff plus the effect's own slot id. The depth is the last corner's z over
 *   12, and every triangle is linked at ordering table entry 0, not at its
 *   depth.
 *
 * A polygon whose depth is not between 1 and EFFECT_OT_DEPTH_MAX - 1 is built but
 * not drawn. */
void battle_effect_draw_projectile_model(
    SVECTOR* spin, VECTOR* position, SVECTOR* angles, VECTOR* scale, s32 parameter) {
    battle_effect_decoded_model_command_t command;
    s32 unused[2];
    MATRIX matrix;
    SVECTOR point;
    psyq_tmd_header_t* model;
    SVECTOR* vertices;
    VECTOR* scratch;
    s32 count;
    s32 i;
    s32 command_count;
    s32 gt3_index;
    s32 g4_index;
    s32 g3_index;
    s32 j;
    s32 z;
    u32* ot;

    RotMatrix(spin, &matrix);
    matrix.t[2] = 0;
    matrix.t[1] = 0;
    matrix.t[0] = 0;
    SetRotMatrix(&matrix);
    SetTransMatrix(&matrix);
    model = g_battle_effect_model_files[parameter & 0xff];
    scratch = SCRATCH;
    vertices = (SVECTOR*)((u8*)model->objects + model->objects[0].vertex_offset);
    count = model->objects[0].vertex_count;
    for (i = 0; i < count; i++) {
        gte_ldv0(&vertices[i]);
        gte_rtv0tr();
        gte_stlvnl(&scratch[i]);
        gte_stflg(&scratch[i].pad);
    }
    RotMatrix(angles, &matrix);
    ScaleMatrixL(&matrix, scale);
    matrix.t[0] = position->vx;
    matrix.t[1] = position->vy;
    matrix.t[2] = position->vz;
    SetRotMatrix(&matrix);
    SetTransMatrix(&matrix);
    for (i = 0; i < count; i++) {
        copyVector(&point, &scratch[i]);
        gte_ldv0(&point);
        gte_rtv0tr();
        gte_stlvnl(&scratch[i]);
        gte_stflg(&scratch[i].pad);
    }
    SetRotMatrix(&g_battle_camera_matrix);
    SetTransMatrix(&g_battle_camera_matrix);
    for (i = 0; i < count; i++) {
        copyVector(&point, &scratch[i]);
        gte_ldv0(&point);
        gte_rtv0tr();
        gte_stlvnl(&scratch[i]);
        gte_stflg(&scratch[i].pad);
    }
    command_count = battle_effect_init_model_sections(model, 0);
    gt3_index = 0;
    g4_index = 0;
    g3_index = 0;
    switch (parameter & 0xff) {
    case BATTLE_EFFECT_MODEL_ARROW: {
        battle_effect_arrow_buffer_t* work;
        POLY_G4* quad;
        POLY_G3* tri;

        work = g_battle_effect_current_secondary->allocation;
        for (i = 0; i < command_count;) {
            battle_effect_decode_next_model_command(&command);
            if (command.header.fields.opcode == BATTLE_EFFECT_MODEL_COMMAND_G4) {
                quad = &work->g4[g_battle_effect_buffer_index][g4_index];
                quad->r0 = command.colors[0].r;
                quad->g0 = command.colors[0].g;
                quad->b0 = command.colors[0].b;
                quad->r1 = command.colors[1].r;
                quad->g1 = command.colors[1].g;
                quad->b1 = command.colors[1].b;
                quad->r2 = command.colors[2].r;
                quad->g2 = command.colors[2].g;
                quad->b2 = command.colors[2].b;
                quad->r3 = command.colors[3].r;
                quad->g3 = command.colors[3].g;
                quad->b3 = command.colors[3].b;
                quad->x0 = PT(0).vx;
                quad->y0 = PT(0).vy;
                z = PT(0).vz;
                quad->x1 = PT(1).vx;
                quad->y1 = PT(1).vy;
                z += PT(1).vz;
                quad->x2 = PT(2).vx;
                quad->y2 = PT(2).vy;
                z += PT(2).vz;
                quad->x3 = PT(3).vx;
                quad->y3 = PT(3).vy;
                g4_index++;
                z += PT(3).vz;
                z /= 16;
                if (z > 0 && z < EFFECT_OT_DEPTH_MAX) {
                    addPrim(g_battle_effect_polygon_depth_data + z, quad);
                }
            }
            i++;
            if (command.header.fields.opcode == BATTLE_EFFECT_MODEL_COMMAND_G3) {
                tri = &work->g3[g_battle_effect_buffer_index][g3_index];
                tri->r0 = command.colors[0].r;
                tri->g0 = command.colors[0].g;
                tri->b0 = command.colors[0].b;
                tri->r1 = command.colors[1].r;
                tri->g1 = command.colors[1].g;
                tri->b1 = command.colors[1].b;
                tri->r2 = command.colors[2].r;
                tri->g2 = command.colors[2].g;
                tri->b2 = command.colors[2].b;
                tri->x0 = PT(0).vx;
                tri->y0 = PT(0).vy;
                z = PT(0).vz;
                tri->x1 = PT(1).vx;
                tri->y1 = PT(1).vy;
                z += PT(1).vz;
                tri->x2 = PT(2).vx;
                tri->y2 = PT(2).vy;
                z += PT(2).vz;
                z /= 12;
                g3_index++;
                if (z > 0 && z < EFFECT_OT_DEPTH_MAX) {
                    addPrim(g_battle_effect_polygon_depth_data + z, tri);
                }
            }
        }
        break;
    }
    case BATTLE_EFFECT_MODEL_STONE: {
        battle_effect_stone_buffer_t* work;
        POLY_G3* tri;

        work = g_battle_effect_current_secondary->allocation;
        j = 0;
        for (i = 0; i < command_count;) {
            battle_effect_decode_next_model_command(&command);
            i++;
            if (command.header.fields.opcode == BATTLE_EFFECT_MODEL_COMMAND_G3) {
                tri = &work->g3[g_battle_effect_buffer_index][j];
                tri->r0 = command.colors[0].r;
                tri->g0 = command.colors[0].g;
                tri->b0 = command.colors[0].b;
                tri->r1 = command.colors[1].r;
                tri->g1 = command.colors[1].g;
                tri->b1 = command.colors[1].b;
                tri->r2 = command.colors[2].r;
                tri->g2 = command.colors[2].g;
                tri->b2 = command.colors[2].b;
                tri->x0 = PT(0).vx;
                tri->y0 = PT(0).vy;
                z = PT(0).vz;
                tri->x1 = PT(1).vx;
                tri->y1 = PT(1).vy;
                z += PT(1).vz;
                tri->x2 = PT(2).vx;
                tri->y2 = PT(2).vy;
                z += PT(2).vz;
                z /= 12;
                j++;
                if (z > 0 && z < EFFECT_OT_DEPTH_MAX) {
                    addPrim(g_battle_effect_polygon_depth_data + z, tri);
                }
            }
        }
        break;
    }
    case BATTLE_EFFECT_MODEL_REFLECT: {
        battle_effect_reflect_buffer_t* work;
        POLY_GT3* tri;

        work = g_battle_effect_current_secondary->allocation;
        /* g4_index is reused here as the limit on decoded commands. */
        g4_index = g_battle_effect_current_secondary->timer * 3;
        if (command_count < g4_index) {
            g4_index = command_count;
        }
        for (i = 0; i < g4_index;) {
            battle_effect_decode_next_model_command(&command);
            i++;
            if (command.header.fields.opcode == BATTLE_EFFECT_MODEL_COMMAND_GT3_37) {
                tri = &work->prims[g_battle_effect_buffer_index][gt3_index];
                tri->tpage = 0x3f;
                tri->clut = g_battle_effect_current_secondary->own_slot_id + 0x7eff;
                tri->r0 = command.colors[0].r;
                tri->g0 = command.colors[0].g;
                tri->b0 = command.colors[0].b;
                tri->r1 = command.colors[1].r;
                tri->g1 = command.colors[1].g;
                tri->b1 = command.colors[1].b;
                tri->r2 = command.colors[2].r;
                tri->g2 = command.colors[2].g;
                tri->b2 = command.colors[2].b;
                memcpy(&tri->u0, &command.texture[2], sizeof(command.texture[2]));
                memcpy(&tri->u1, &command.texture[3], sizeof(command.texture[3]));
                memcpy(&tri->u2, &command.texture[4], sizeof(command.texture[4]));
                tri->x0 = PT(0).vx;
                tri->y0 = PT(0).vy;
                z = PT(0).vz;
                tri->x1 = PT(1).vx;
                tri->y1 = PT(1).vy;
                z = PT(1).vz;
                tri->x2 = PT(2).vx;
                tri->y2 = PT(2).vy;
                z = PT(2).vz;
                z /= 12;
                gt3_index++;
                if (z > 0 && z < EFFECT_OT_DEPTH_MAX) {
                    ot = g_battle_effect_polygon_depth_data;
                    addPrim(ot, tri);
                }
            }
        }
        break;
    }
    case BATTLE_EFFECT_MODEL_SHURIKEN: {
        battle_effect_shuriken_buffer_t* work;
        POLY_G3* tri;

        work = g_battle_effect_current_secondary->allocation;
        j = 0;
        for (i = 0; i < command_count;) {
            battle_effect_decode_next_model_command(&command);
            i++;
            if (command.header.fields.opcode == BATTLE_EFFECT_MODEL_COMMAND_G3) {
                tri = &work->g3[g_battle_effect_buffer_index][j];
                tri->r0 = command.colors[0].r;
                tri->g0 = command.colors[0].g;
                tri->b0 = command.colors[0].b;
                tri->r1 = command.colors[1].r;
                tri->g1 = command.colors[1].g;
                tri->b1 = command.colors[1].b;
                tri->r2 = command.colors[2].r;
                tri->g2 = command.colors[2].g;
                tri->b2 = command.colors[2].b;
                tri->x0 = PT(0).vx;
                tri->y0 = PT(0).vy;
                z = PT(0).vz;
                tri->x1 = PT(1).vx;
                tri->y1 = PT(1).vy;
                z += PT(1).vz;
                tri->x2 = PT(2).vx;
                tri->y2 = PT(2).vy;
                z += PT(2).vz;
                z /= 12;
                j++;
                if (z > 0 && z < EFFECT_OT_DEPTH_MAX) {
                    addPrim(g_battle_effect_polygon_depth_data + z, tri);
                }
            }
        }
        break;
    }
    }
}
