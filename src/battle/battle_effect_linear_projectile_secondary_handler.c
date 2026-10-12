#include "fft/battle.h"
#include "fft/battle_effect.h"
#include "psx/libgpu.h"
#include "psx/libgte.h"

/* Secondary effect function 0x14 in g_battle_effect_secondary_handlers: a
 * projectile flying in a straight line. It runs the crossbow bolt (the arrow
 * model), the thrown stone and the thrown item animations.
 *
 * Initializing measures the trajectory, takes the flight time from
 * g_battle_effect_trajectory_step_count and allocates what the projectile draws
 * with: the polygon buffer of the arrow or stone model, or for a thrown item the
 * shuriken model's buffer (items ITEM_ID_SHURIKEN to ITEM_ID_YAGYU_DARKNESS) or
 * else an effect slot holding the item's icon as a quad. Executing moves 0x8000
 * (8 units in Q12) along the line each frame, aims the model along it and draws
 * it: the arrow at scale 0x40, the stone and the shuriken at 0xc0 and spinning by
 * the frames left, any other item as its icon sprite.
 *
 * With two frames left and a unit hit by the trajectory
 * (g_battle_effect_trajectory_hit_unit_id), the target's hit reaction
 * (battle_unit_call_bow_hardcoding_by_misc_id) starts, unless the item is one of
 * the balls or a consumable (ITEM_ID_POTION to ITEM_ID_PHOENIX_DOWN). When the
 * count runs out the icon slot is freed; a unit target whose result is neither
 * MISSED nor NO_EFFECT releases the requested ability effect at animation speed
 * 2, and otherwise a requested effect is dropped after the hit reaction. The
 * handler then enters Finalizing, whose return of 0 removes it.
 *
 * g_battle_effect_projectile_spin_passes is the count of frames left. result is
 * unset for a phase outside 1-3, and scale and spin for a model other than the
 * arrow or the stone. */
s32 battle_effect_linear_projectile_secondary_handler(void) {
    s32 unused[2];
    VECTOR delta;
    VECTOR position;
    SVECTOR angles;
    VECTOR scale;
    SVECTOR spin;
    SPRT sprite;
    battle_effect_arrow_buffer_t* arrow;
    battle_effect_stone_buffer_t* work12;
    battle_effect_shuriken_buffer_t* work16;
    battle_effect_slot_t* slot;
    s32 parameter;
    s32 item;
    s32 i;
    s32 j;
    s32 result;
    s32 kind;
    POLY_FT4* dst;

    switch (g_battle_effect_current_secondary->phase) {
    case BATTLE_SECONDARY_EFFECT_INITIALIZING:
        setVector(&delta, g_battle_effect_trajectory_destination.vx - g_battle_effect_trajectory_origin.vx,
            g_battle_effect_trajectory_destination.vy - g_battle_effect_trajectory_origin.vy,
            g_battle_effect_trajectory_destination.vz - g_battle_effect_trajectory_origin.vz);
        g_battle_effect_projectile_length
            = SquareRoot12((delta.vx * delta.vx + delta.vy * delta.vy + delta.vz * delta.vz) << 12);
        g_battle_effect_projectile_delta = delta;
        g_battle_effect_projectile_progress = 0;
        g_battle_effect_projectile_speed = 0x8000;
        g_battle_effect_projectile_spin_passes = g_battle_effect_trajectory_step_count;
        g_battle_effect_current_secondary->phase = BATTLE_SECONDARY_EFFECT_EXECUTING;
        if (g_battle_effect_current_secondary->allocation != 0) {
            main_heap_free(g_battle_effect_current_secondary->allocation);
        }
        parameter = g_battle_effect_secondary_animations[g_battle_effect_current_secondary->animation_id].parameter;
        if ((parameter & 0xff) == BATTLE_EFFECT_MODEL_ARROW) {
            arrow = main_heap_alloc(0xc88);
            g_battle_effect_current_secondary->allocation = arrow;
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 4; j++) {
                    SetPolyG3(&arrow->g3[i][j]);
                }
                for (j = 0; j < 13; j++) {
                    SetPolyG4(&arrow->g4[i][j]);
                }
            }
        }
        if ((parameter & 0xff) == BATTLE_EFFECT_MODEL_STONE) {
            work12 = main_heap_alloc(0x2a0);
            g_battle_effect_current_secondary->allocation = work12;
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 12; j++) {
                    SetPolyG3(&work12->g3[i][j]);
                }
            }
        }
        if (parameter == BATTLE_EFFECT_SECONDARY_PARAMETER_ITEM_GRAPHIC) {
            item = g_battle_effect_current_secondary->used_weapon_id;
            if (item >= ITEM_ID_SHURIKEN && item <= ITEM_ID_YAGYU_DARKNESS) {
                work16 = main_heap_alloc(0x380);
                g_battle_effect_current_secondary->allocation = work16;
                for (i = 0; i < 2; i++) {
                    for (j = 0; j < 16; j++) {
                        SetPolyG3(&work16->g3[i][j]);
                    }
                }
            } else {
                battle_gfx_set_item_icon_sprite(&sprite, item);
                g_battle_effect_current_secondary->slot_ids[0] = battle_effect_alloc_slot();
                slot = &g_battle_effect_slots[g_battle_effect_current_secondary->slot_ids[0]];
                dst = &slot->quads[1];
                SetPolyFT4(&slot->quads[0]);
                slot->quads[0].tpage = 0x1e;
                slot->quads[0].b0 = 0x80;
                slot->quads[0].g0 = 0x80;
                slot->quads[0].r0 = 0x80;
                slot->quads[0].u0 = slot->quads[0].u2 = sprite.u0;
                slot->quads[0].u1 = slot->quads[0].u3 = slot->quads[0].u0 + sprite.w;
                slot->quads[0].v0 = slot->quads[0].v1 = sprite.v0;
                slot->quads[0].v2 = slot->quads[0].v3 = slot->quads[0].v0 + sprite.h;
                slot->quads[0].clut = sprite.clut;
                *dst = slot->quads[0];
            }
        }
        battle_state_set_animation_speed(1);
        result = 1;
        break;
    case BATTLE_SECONDARY_EFFECT_EXECUTING:
        g_battle_effect_projectile_progress += g_battle_effect_projectile_speed;
        battle_effect_find_projectile_xyz_along_trajectory(&g_battle_effect_projectile_delta,
            g_battle_effect_projectile_length, g_battle_effect_projectile_progress, &position);
        setVector(&position, g_battle_effect_trajectory_origin.vx + position.vx,
            g_battle_effect_trajectory_origin.vy + position.vy, g_battle_effect_trajectory_origin.vz + position.vz);
        delta.vx = g_battle_effect_trajectory_destination.vx - g_battle_effect_trajectory_origin.vx;
        delta.vz = g_battle_effect_trajectory_destination.vz - g_battle_effect_trajectory_origin.vz;
        delta.vy = g_battle_effect_trajectory_destination.vy - g_battle_effect_trajectory_origin.vy;
        angles.vy = ratan2(-delta.vz, delta.vx);
        angles.vz = ratan2(delta.vy, SquareRoot0(delta.vx * delta.vx + delta.vz * delta.vz)) + 0x400;
        angles.vx = 0;
        parameter = g_battle_effect_secondary_animations[g_battle_effect_current_secondary->animation_id].parameter;
        i = parameter & 0xff;
        if (parameter != BATTLE_EFFECT_SECONDARY_PARAMETER_ITEM_GRAPHIC) {
            if (i == BATTLE_EFFECT_MODEL_ARROW) {
                scale.vz = 0x40;
                scale.vy = 0x40;
                scale.vx = 0x40;
                spin.vz = 0;
                spin.vy = 0;
                spin.vx = 0;
            } else if (i == BATTLE_EFFECT_MODEL_STONE) {
                scale.vz = 0xc0;
                scale.vy = 0xc0;
                scale.vx = 0xc0;
                setVector(&spin, 0, g_battle_effect_projectile_spin_passes * 256,
                    g_battle_effect_projectile_spin_passes * 128);
            }
            battle_effect_draw_projectile_model(&spin, &position, &angles, &scale, parameter);
        } else {
            item = g_battle_effect_current_secondary->used_weapon_id;
            if (item >= ITEM_ID_SHURIKEN && item <= ITEM_ID_YAGYU_DARKNESS) {
                scale.vz = 0xc0;
                scale.vy = 0xc0;
                scale.vx = 0xc0;
                spin.vy = 0;
                spin.vx = 0;
                spin.vz = -(g_battle_effect_projectile_spin_passes * 256);
                battle_effect_draw_projectile_model(&spin, &position, &angles, &scale, BATTLE_EFFECT_MODEL_SHURIKEN);
            } else {
                battle_effect_draw_item_drop_sprite(&position, item, 0);
            }
        }
        if (g_battle_effect_projectile_spin_passes == 2 && g_battle_effect_trajectory_hit_unit_id != -1) {
            kind = g_battle_effect_current_secondary->used_weapon_id;
            if ((kind < ITEM_ID_FIRE_BALL || kind > ITEM_ID_LIGHTNING_BALL)
                && (kind < ITEM_ID_POTION || kind > ITEM_ID_PHOENIX_DOWN)
                && g_battle_effect_current_secondary->target_count != 0) {
                battle_unit_call_bow_hardcoding_by_misc_id(g_battle_effect_current_secondary->caster.data.unit.misc_id,
                    g_battle_effect_current_secondary->target.record.data.unit.misc_id);
            }
        }
        if (g_battle_effect_projectile_spin_passes > 0) {
            g_battle_effect_projectile_spin_passes--;
        }
        if (g_battle_effect_projectile_spin_passes == 0) {
            if (g_battle_effect_current_secondary->slot_ids[0] != 0) {
                battle_effect_free_slot(g_battle_effect_current_secondary->slot_ids[0]);
                g_battle_effect_current_secondary->slot_ids[0] = 0;
            }
            if (g_battle_effect_current_secondary->target.record.target_type == BATTLE_EFFECT_TARGET_TYPE_UNIT
                && g_battle_effect_current_secondary->target.record.data.unit.result_animation
                    != BATTLE_EFFECT_HIT_MISSED
                && g_battle_effect_current_secondary->target.record.data.unit.result_animation
                    != BATTLE_EFFECT_HIT_NO_EFFECT) {
                battle_effect_play();
                battle_state_set_animation_speed(2);
            } else if (battle_effect_get_phase() == BATTLE_EFFECT_PHASE_REQUESTED) {
                if (g_battle_effect_current_secondary->target_count != 0) {
                    battle_unit_call_bow_hardcoding_by_misc_id(
                        g_battle_effect_current_secondary->caster.data.unit.misc_id,
                        g_battle_effect_current_secondary->target.record.data.unit.misc_id);
                }
                battle_effect_set_phase_none();
            }
            g_battle_effect_current_secondary->phase = BATTLE_SECONDARY_EFFECT_FINALIZING;
        }
        result = 1;
        break;
    case BATTLE_SECONDARY_EFFECT_FINALIZING:
        result = 0;
        break;
    }
    return result;
}
