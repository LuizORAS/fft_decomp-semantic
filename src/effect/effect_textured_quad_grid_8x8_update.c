#include "fft/effect.h"
#include "psx/libgpu.h"
#include "psx/libgte.h"
#include "psx/types.h"

/*
 * Phase handler for the scrolling 8x8 textured quad grid.
 *
 * The 8x8 build of effect_e047_update_textured_quad_grid_4x4 (E047): 32-pixel
 * cells, nine brightness entries per row and an ordering-table depth of at
 * least 4. The shared row/column scratch variables are explained there.
 */
s32 effect_textured_quad_grid_8x8_update(s32 record_index, s32 callback_slot, s32 emitter_index, s32 frame) {
    s16 state_index = record_index;
    effect_record_t* record;
    s32 translucent;
    u16 texture_page;
    effect_texture_page_config_t configuration;
    s32 texture_width;
    s32 texture_height;
    u32* ot;
    effect_textured_quad_geometry_view_t* geometry;
    effect_textured_quad_grid_8x8_work_t* work;
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

    record = &g_battle_effect_state_records[state_index];
    geometry = &g_battle_effect_geometry_table->entries[emitter_index].textured_quad;
    ot = main_gfx_get_otag();

    switch (record->callback_phase[callback_slot]) {
    case EFFECT_PHASE_IDLE:
        break;
    case EFFECT_PHASE_INIT:
        if (record->work_slots[callback_slot] != 0) {
            battle_heap_free_block(record->work_slots[callback_slot]);
        }
        work = battle_heap_alloc_block(sizeof(effect_textured_quad_grid_8x8_work_t), state_index);
        record->work_slots[callback_slot] = work;
        configuration = geometry->texture_page_config;
        if (configuration.clear_abe) {
            translucent = 0;
        } else {
            translucent = 1;
        }
        texture_page = (configuration.blend_mode << 5) | 0x86;
        for (row = 0; row < 8; row++) {
            for (column = 0; column < 8; column++) {
                quad = &work->quads[0][row * 8 + column];
                SetPolyGT4(quad);
                setSemiTrans(quad, translucent);
                quad->tpage = texture_page;
                quad->clut = MAP_CLUT(MAP_CLUT_ROW_EFFECT, 0);
                quad->x0 = quad->x2 = (column << 5) + 128;
                quad->y0 = quad->y1 = row << 5;
                quad->x1 = quad->x3 = (column << 5) + 160;
                quad->y2 = quad->y3 = (row << 5) + 32;
                work->quads[1][row * 8 + column] = *quad;
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
        BATTLE_EFFECT_READ_CURVE(
            g_battle_effect_animation_curve_data, row, column, geometry->curve_selectors[0] >> 28, work->frame);
        speed = battle_effect_lerp_linear(geometry->scroll_speed_start, geometry->scroll_speed_end, column) << 4;
        BATTLE_EFFECT_READ_CURVE(
            g_battle_effect_animation_curve_data, row, column, (geometry->curve_selectors[0] >> 8) & 0xf, work->frame);
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
        blue = EFFECT_COLOR_NEUTRAL;
        if (geometry->flags & EFFECT_EMITTER_FLAG_COLOUR_CURVES) {
            row = geometry->colour_curves & 0xfff;
            red = g_battle_effect_animation_curve_data->curves[row & 0xf][work->frame];
            green = g_battle_effect_animation_curve_data->curves[(row >> 4) & 0xf][work->frame];
            blue = g_battle_effect_animation_curve_data->curves[(row >> 8) & 0xf][work->frame];
        } else {
            green = EFFECT_COLOR_NEUTRAL;
            red = EFFECT_COLOR_NEUTRAL;
        }
        depth = geometry->ot_depth;
        brightness_row = geometry->brightness_row;
        if (depth < EFFECT_OT_DEPTH_MIN) {
            depth = EFFECT_OT_DEPTH_MIN;
        }
        if (depth >= EFFECT_OT_DEPTH_LIMIT) {
            depth = EFFECT_OT_DEPTH_MAX;
        }
        for (quad_row = 0; quad_row < 8; quad_row++) {
            for (quad_column = 0; quad_column < 8; quad_column++) {
                quad = &work->quads[work->buffer][quad_row * 8 + quad_column];
                u = (work->scroll_u >> 12) + work->texture_u;
                quad->u0 = quad->u2 = u;
                quad->u1 = quad->u3 = u + texture_width;
                v = (work->scroll_v >> 12) + work->texture_v;
                quad->v0 = quad->v1 = v;
                quad->v2 = quad->v3 = v + texture_height;
                row = g_effect_textured_quad_grid_8x8_brightness_table[brightness_row][quad_row];
                setRGB0(
                    quad, quad->r1 = (row * red) >> 12, quad->g1 = (row * green) >> 12, quad->b1 = (row * blue) >> 12);
                row = g_effect_textured_quad_grid_8x8_brightness_table[brightness_row][quad_row + 1];
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
