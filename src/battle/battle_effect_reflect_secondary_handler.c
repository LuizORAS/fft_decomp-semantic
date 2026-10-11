#include "fft/battle.h"
#include "psx/libgpu.h"
#include "psx/libgte.h"

/* Secondary-effect handler 0x12 (Reflect). The effect's parameter is the
 * frames the shield stays, at least 26: the child timeline passes the
 * effect's spawn_delay.
 *
 * Phase 1 allocates and initialises the work buffer; phase 2 advances a
 * two-entry CLUT wipe near the end of the effect, uploads the CLUT, aims the
 * shield from the target's centre toward the caster and hands the transform
 * to battle_effect_draw_projectile_model; phase 3 finishes. The wipe indices are assigned inside the
 * subscript, each to its own variable: a separate statement or a folded
 * `timer - 1 - span` changes the order and association of the index
 * arithmetic. spin_timer keeps the timer load at full width (lh). */
s32 battle_effect_reflect_secondary_handler(void) {
    battle_effect_secondary_data_t* effect;
    s32 unused[2];
    VECTOR delta;
    VECTOR position;
    SVECTOR angles;
    VECTOR scale;
    SVECTOR spin;
    RECT rect;
    battle_effect_reflect_buffer_t* work;
    SVECTOR* caster;
    SVECTOR* target;
    rgb24_t* colors;
    u8* blue;
    u8* green;
    u8* red;
    s32 span;
    s32 timer;
    s32 lit;
    s32 cleared;
    s32 spin_timer;
    s32 parameter;
    s32 i;
    s32 j;
    s32 result;

    span = g_battle_effect_current_secondary->parameter;
    if (span < 0x1a) {
        span = 0x1a;
    }
    switch (g_battle_effect_current_secondary->phase) {
    case BATTLE_SECONDARY_EFFECT_INITIALIZING:
        if (g_battle_effect_current_secondary->allocation != 0) {
            main_heap_free(g_battle_effect_current_secondary->allocation);
        }
        work = main_heap_alloc(0x798);
        g_battle_effect_current_secondary->allocation = work;
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 0x18; j++) {
                SetPolyGT3(&work->prims[i][j]);
                SetSemiTrans(&work->prims[i][j], 1);
            }
        }
        /* Separate channel pointers retain the target's three stride bases. */
        for (i = 0; i < 2; i++) {
            colors = g_battle_effect_reflect_palette_colors;
            red = &colors->r;
            green = &colors->g;
            blue = &colors->b;
            for (j = 0; j < 6; j++) {
                work->clut[i][j] = red[j * sizeof(*colors)] | (green[j * sizeof(*colors)] << 5)
                    | (blue[j * sizeof(*colors)] << 10) | 0x8000;
            }
        }
        result = 1;
        g_battle_effect_current_secondary->timer = 0;
        g_battle_effect_current_secondary->phase = BATTLE_SECONDARY_EFFECT_EXECUTING;
        break;
    case BATTLE_SECONDARY_EFFECT_EXECUTING:
        work = g_battle_effect_current_secondary->allocation;
        timer = g_battle_effect_current_secondary->timer;
        if (span < timer && timer < span + 4) {
            work->clut[g_battle_effect_buffer_index][((lit = timer - 1) - span) * 2]
                = work->clut[g_battle_effect_buffer_index][(lit - span) * 2 + 1] = 0xffff;
        }
        if (span + 1 < g_battle_effect_current_secondary->timer) {
            work->clut[g_battle_effect_buffer_index][((cleared = g_battle_effect_current_secondary->timer - 2) - span)
                * 2] = work->clut[g_battle_effect_buffer_index][(cleared - span) * 2 + 1] = 0;
        }
        rect.x = (g_battle_effect_current_secondary->own_slot_id - 1) * 16 + 1;
        rect.y = 0x1fc;
        rect.w = 6;
        rect.h = 1;
        LoadImage(&rect, (u32*)work->clut[g_battle_effect_buffer_index]);
        caster
            = battle_unit_get_screen_data_ptr_by_misc_id(g_battle_effect_current_secondary->caster.data.unit.misc_id);
        target = battle_unit_get_screen_data_ptr_by_misc_id(
            g_battle_effect_current_secondary->target.record.data.unit.misc_id);
        setVector(&delta, caster->vx - target->vx, caster->vy - target->vy, caster->vz - target->vz);
        position.vx = target->vx;
        position.vz = target->vz;
        position.vy = target->vy
            - (s32)battle_gfx_get_unit_spritesheet_height_by_misc_id(
                  g_battle_effect_current_secondary->target.record.data.unit.misc_id)
                / 2;
        angles.vy = ratan2(-delta.vz, delta.vx);
        angles.vz = ratan2(delta.vy, SquareRoot0(delta.vx * delta.vx + delta.vz * delta.vz)) + 0x400;
        angles.vx = 0;
        scale.vz = 0x140;
        scale.vy = 0x140;
        scale.vx = 0x140;
        parameter = g_battle_effect_secondary_animations[g_battle_effect_current_secondary->animation_id].parameter;
        spin.vz = 0;
        spin.vx = 0;
        spin_timer = g_battle_effect_current_secondary->timer;
        spin.vz = spin_timer << 7;
        battle_effect_draw_projectile_model(&spin, &position, &angles, &scale, parameter);
        effect = g_battle_effect_current_secondary;
        effect->timer++;
        if (span + 4 < effect->timer) {
            effect->phase = BATTLE_SECONDARY_EFFECT_FINALIZING;
        }
        result = 1;
        break;
    case BATTLE_SECONDARY_EFFECT_FINALIZING:
        result = 0;
        break;
    }
    return result;
}
