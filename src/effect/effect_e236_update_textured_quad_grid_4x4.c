/* E236.BIN is the effect of ABILITY_ID_JA_MAGIC_BLIND_2. */
#include "fft/effect.h"
#include "psx/libgpu.h"
#include "psx/libgte.h"
#include "psx/types.h"

/*
 * Phase handler for the scrolling 4x4 textured quad grid.
 *
 * Phase 1 builds both POLY_GT4 buffers of a 256x256 grid; phase 2 scrolls the
 * texture window along the emitter record's speed and angle curves, tints each
 * quad from the colour curves and the overlay-local vertex brightness rows, and
 * links the current buffer into the ordering table; phase 3 frees the work
 * block one call after it is first reached.
 *
 * row and column are also the update phase's curve scratch values. GCC 2.6.3
 * gives each C variable one pseudo, and that sharing is what places the curve
 * index, colour curves and brightness in s2 and the curve factor in s3.
 */
s32 effect_e236_update_textured_quad_grid_4x4(s16 record_index, s32 callback_slot, s32 emitter_index, s32 frame) {
    effect_record_t* record;
    s32 translucent;
    u16 texture_page;
    s32 texture_width;
    s32 texture_height;
    u32* ot;
    effect_textured_quad_geometry_view_t* geometry;
    effect_textured_quad_grid_4x4_work_t* work;
    POLY_GT4* quad;
    s32 row;
    s32 column;
    s32 quad_column;
    s32 quad_row;
    s32 speed;
    s16 angle;
    u32 red;
    u32 green;
    u32 blue;
    s32 depth;
    s32 brightness_row;
    u8 u;
    u8 v;

    record = &g_battle_effect_state_records[record_index];
    geometry = &g_battle_effect_geometry_table->entries[emitter_index].textured_quad;
    ot = main_gfx_get_otag();

    switch (record->callback_phase[callback_slot]) {
    case EFFECT_PHASE_IDLE:
        break;
    case EFFECT_PHASE_INIT:
        if (record->work_slots[callback_slot] != 0) {
            battle_heap_free_block(record->work_slots[callback_slot]);
        }
        work = battle_heap_alloc_block(sizeof(effect_textured_quad_grid_4x4_work_t), record_index);
        record->work_slots[callback_slot] = work;
        if (geometry->texture_page_flags & EFFECT_TEXTURE_PAGE_FLAG_CLEAR_ABE) {
            translucent = 0;
        } else {
            translucent = 1;
        }
        texture_page = ((geometry->texture_page_flags & EFFECT_TEXTURE_PAGE_ABR_MASK) << 5) | 0x86;
        for (row = 0; row < 4; row++) {
            for (column = 0; column < 4; column++) {
                quad = &work->quads[0][row * 4 + column];
                SetPolyGT4(quad);
                if (translucent) {
                    quad->code |= 2;
                } else {
                    quad->code &= 0xfd;
                }
                quad->tpage = texture_page;
                quad->clut = MAP_CLUT(MAP_CLUT_ROW_EFFECT, 0);
                quad->x0 = quad->x2 = (column << 6) + 0x80;
                quad->y0 = quad->y1 = row << 6;
                quad->x1 = quad->x3 = (column << 6) + 0xc0;
                quad->y2 = quad->y3 = (row << 6) + 0x40;
                work->quads[1][row * 4 + column] = *quad;
            }
        }
        work->scroll_v = 0;
        work->scroll_u = 0;
        work->frame = 0;
        work->buffer = 0;
        work->destroy_latched = 0;
        work->texture_u = geometry->texture_u;
        work->texture_v = geometry->texture_v;
        record->callback_phase[callback_slot] = EFFECT_PHASE_UPDATE;
        break;
    case EFFECT_PHASE_UPDATE:
        work = record->work_slots[callback_slot];
        texture_width = geometry->texture_width / 2;
        texture_height = geometry->texture_height / 2;
        /* Two statements keep the target's li -1 / beq compare. */
        row = geometry->curve_indices[0] >> 28;
        row -= 1;
        if (row != -1) {
            column = g_battle_effect_animation_curve_data->curves[row][work->frame];
        } else {
            column = 0;
        }
        speed = battle_effect_lerp_linear(geometry->scroll_speed_start, geometry->scroll_speed_end, column) << 4;
        row = (geometry->curve_indices[0] >> 8) & 0xf;
        row -= 1;
        if (row != -1) {
            column = g_battle_effect_animation_curve_data->curves[row][work->frame];
        } else {
            column = 0;
        }
        angle = battle_effect_lerp_linear(geometry->scroll_angle_start, geometry->scroll_angle_end, column);
        work->scroll_u += (rcos(angle) * speed) >> 12;
        while (work->scroll_u >= texture_width << 12) {
            work->scroll_u -= texture_width << 12;
        }
        while (work->scroll_u < 0) {
            work->scroll_u += texture_width << 12;
        }
        work->scroll_v += (rsin(angle) * speed) >> 12;
        while (work->scroll_v >= texture_height << 12) {
            work->scroll_v -= texture_height << 12;
        }
        while (work->scroll_v < 0) {
            work->scroll_v += texture_height << 12;
        }
        blue = 0x80;
        if (geometry->flags & EFFECT_EMITTER_FLAG_COLOUR_CURVES) {
            row = geometry->colour_curves & 0xfff;
            red = g_battle_effect_animation_curve_data->curves[row & 0xf][work->frame];
            green = g_battle_effect_animation_curve_data->curves[(row >> 4) & 0xf][work->frame];
            blue = g_battle_effect_animation_curve_data->curves[(row >> 8) & 0xf][work->frame];
        } else {
            green = 0x80;
            red = 0x80;
        }
        depth = geometry->ot_depth;
        brightness_row = geometry->brightness_row;
        if (depth <= 0) {
            depth = 1;
        }
        if (depth >= EFFECT_OT_DEPTH_LIMIT) {
            depth = EFFECT_OT_DEPTH_MAX;
        }
        for (quad_row = 0; quad_row < 4; quad_row++) {
            for (quad_column = 0; quad_column < 4; quad_column++) {
                quad = &work->quads[work->buffer][quad_row * 4 + quad_column];
                u = (work->scroll_u >> 12) + work->texture_u;
                quad->u0 = quad->u2 = u;
                quad->u1 = quad->u3 = u + texture_width;
                v = (work->scroll_v >> 12) + work->texture_v;
                quad->v0 = quad->v1 = v;
                quad->v2 = quad->v3 = v + texture_height;
                row = g_effect_gfx_brightness_table[brightness_row][quad_row];
                setRGB0(
                    quad, quad->r1 = (row * red) >> 12, quad->g1 = (row * green) >> 12, quad->b1 = (row * blue) >> 12);
                row = g_effect_gfx_brightness_table[brightness_row][quad_row + 1];
                setRGB2(
                    quad, quad->r3 = (row * red) >> 12, quad->g3 = (row * green) >> 12, quad->b3 = (row * blue) >> 12);
                AddPrim(ot + depth, quad);
            }
        }
        work->frame++;
        work->buffer = 1 - work->buffer;
        break;
    case EFFECT_PHASE_DESTROY:
        work = record->work_slots[callback_slot];
        if (work->destroy_latched == 0) {
            work->destroy_latched = 1;
        } else {
            if (work != 0) {
                battle_heap_free_block(work);
                record->work_slots[callback_slot] = 0;
            }
            record->callback_phase[callback_slot] = EFFECT_PHASE_IDLE;
        }
        break;
    }
}
