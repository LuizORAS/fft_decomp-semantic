#include "fft/battle_camera.h"
#include "fft/battle_effect.h"
#include "fft/battle_gfx.h"
#include "fft/battle_map.h"
#include "fft/main.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* 0x28-byte heap state kept in the secondary effect's allocation. */
typedef struct battle_effect_burst_state {
    s32 frame;    /* 0x00 */
    s32 done;     /* 0x04 */
    s32 count_a;  /* 0x08 */
    u8 ids_a[8];  /* 0x0c; group 0 slots */
    s32 count_b;  /* 0x14 */
    u8 ids_b[16]; /* 0x18; group 1 or 9 slots */
} battle_effect_burst_state_t;

/* Secondary effect function 0x02 in g_battle_effect_secondary_handlers: the burst of a weapon
 * hit (BATTLE_EFFECT_SECONDARY_ANIMATION_WEAPON_HIT). */
s32 battle_effect_weapon_hit_secondary_handler(void) {
    battle_effect_secondary_data_t* effect;
    battle_effect_burst_state_t* state;
    battle_effect_slot_t* slot;
    s32 unused[8];
    vec3s32_t gravity;
    VECTOR zoom;
    VECTOR offset;
    s32 result;
    s32 group;
    s32 j;
    s32 done;
    s32 i;
    u8 id;

    effect = g_battle_effect_current_secondary;
    /* Preserve the target's unsigned phase-dispatch comparison. */
    switch (effect->phase) {
    case BATTLE_SECONDARY_EFFECT_INITIALIZING:
        if (effect->target.record.data.unit.result_animation == BATTLE_EFFECT_HIT_NO_EFFECT) {
            return 0;
        }
        if (effect->allocation != 0) {
            main_heap_free(effect->allocation);
        }
        state = main_heap_alloc(sizeof(battle_effect_burst_state_t));
        g_battle_effect_current_secondary->allocation = state;
        state->frame = 0;
        state->done = 0;
        state->count_a = 0;
        state->count_b = 0;
        for (i = 0; i < 8; i++) {
            state->ids_a[i] = 0;
        }
        for (i = 0; i < 16; i++) {
            state->ids_b[i] = 0;
        }
        result = 1;
        g_battle_effect_current_secondary->phase = BATTLE_SECONDARY_EFFECT_EXECUTING;
        battle_effect_copy_camera_angles_and_screen_coords();
        break;
    case BATTLE_SECONDARY_EFFECT_EXECUTING:
        state = effect->allocation;
        group = BATTLE_EFFECT_PARTICLE_GROUP_WEAPON_HIT_B;
        if (effect->target.record.data.unit.result_animation == BATTLE_EFFECT_HIT_MISSED) {
            group = BATTLE_EFFECT_PARTICLE_GROUP_WEAPON_HIT_B_MISSED;
        }
        if (group == BATTLE_EFFECT_PARTICLE_GROUP_WEAPON_HIT_B) {
            if (state->frame == 0) {
                battle_gfx_start_misc_unit_palette_modulation(
                    MAP_COLOR_MODE_BASE_HALF, 0, effect->target.record.data.unit.misc_id, 0x1f, 0x1f, 0x1f);
            }
            if (state->frame == group) {
                battle_gfx_start_misc_unit_palette_modulation(MAP_COLOR_MODE_RESTORE, 2,
                    g_battle_effect_current_secondary->target.record.data.unit.misc_id, 0, 0, 0);
            }
        }
        if (state->frame < 4) {
            if (state->count_a < 8) {
                for (i = 0; i
                    < g_battle_effect_trap_particle_configs[BATTLE_EFFECT_PARTICLE_GROUP_WEAPON_HIT_A].spawns_per_frame;
                    i++) {
                    for (j = 0; j < 8; j++) {
                        if (state->ids_a[j] == 0) {
                            break;
                        }
                    }
                    state->ids_a[j] = battle_effect_alloc_slot();
                    if (state->ids_a[j] == 0) {
                        break;
                    }
                    slot = &g_battle_effect_slots[state->ids_a[j]];
                    slot->life = battle_effect_spawn_particle_motion(
                        &g_battle_effect_trap_particle_configs[BATTLE_EFFECT_PARTICLE_GROUP_WEAPON_HIT_A],
                        &slot->motion);
                    battle_effect_init_trap_animation(BATTLE_EFFECT_PARTICLE_GROUP_WEAPON_HIT_A,
                        g_battle_effect_current_secondary->parameter + MAP_CLUT(MAP_CLUT_ROW_WEAPONS, 0), slot);
                    state->count_a++;
                    if (state->count_a == 8) {
                        break;
                    }
                }
            }
            if (state->frame < 4 && state->count_b < 16) {
                for (i = 0; i < g_battle_effect_trap_particle_configs[group].spawns_per_frame; i++) {
                    for (j = 0; j < 16; j++) {
                        if (state->ids_b[j] == 0) {
                            break;
                        }
                    }
                    state->ids_b[j] = battle_effect_alloc_slot();
                    if (state->ids_b[j] == 0) {
                        break;
                    }
                    slot = &g_battle_effect_slots[state->ids_b[j]];
                    slot->life = battle_effect_spawn_particle_motion(
                        &g_battle_effect_trap_particle_configs[group], &slot->motion);
                    battle_effect_init_trap_animation(
                        BATTLE_EFFECT_PARTICLE_GROUP_WEAPON_HIT_B, MAP_CLUT(MAP_CLUT_ROW_WEAPONS, 10), slot);
                    slot->red = slot->green = slot->blue = 0xff;
                    state->count_b++;
                    if (state->count_b == 16) {
                        break;
                    }
                }
            }
        }
        /* fallthrough */
    case BATTLE_SECONDARY_EFFECT_FINALIZING:
        battle_effect_save_inertia_threshold();
        battle_effect_set_inertia_threshold(BATTLE_EFFECT_SECONDARY_INERTIA_THRESHOLD);
        gravity.z = 0;
        gravity.x = 0;
        gravity.y = ONE;
        battle_effect_save_gravity();
        battle_effect_set_gravity(&gravity);
        for (i = 0; i < 8; i++) {
            id = state->ids_a[i];
            if (id != 0 && battle_effect_update_slot(id) == 0) {
                battle_effect_free_slot(id);
                state->ids_a[i] = 0;
                state->count_a--;
            }
        }
        for (i = 0; i < 16; i++) {
            id = state->ids_b[i];
            if (id != 0 && battle_effect_update_slot(id) == 0) {
                battle_effect_free_slot(id);
                state->ids_b[i] = 0;
                state->count_b--;
            }
        }
        battle_effect_restore_inertia_threshold();
        battle_effect_restore_gravity();
        if (g_battle_effect_current_secondary->target.record.data.unit.result_animation == BATTLE_EFFECT_HIT_CRITICAL) {
            if (state->frame == 0) {
                zoom.vx = zoom.vy = zoom.vz = -0x80;
                battle_camera_execute_zoom_command(
                    BATTLE_EFFECT_CAMERA_SOURCE_SAVED | BATTLE_EFFECT_CAMERA_MODE_COSINE, 0, 2, &zoom);
            }
            if (state->frame == 2) {
                zoom.vx = zoom.vy = zoom.vz = 0x2e4;
                battle_camera_execute_zoom_command(
                    BATTLE_EFFECT_CAMERA_SOURCE_SAVED | BATTLE_EFFECT_CAMERA_MODE_COSINE, 0, 2, &zoom);
            }
            if (state->frame == 4) {
                zoom.vx = zoom.vy = zoom.vz = 0;
                battle_camera_execute_zoom_command(
                    BATTLE_EFFECT_CAMERA_SOURCE_SAVED | BATTLE_EFFECT_CAMERA_MODE_COSINE, 0, 12, &zoom);
                offset.vx = offset.vy = offset.vz = 4;
                battle_camera_execute_position_command(
                    BATTLE_EFFECT_CAMERA_SOURCE_GIVEN | BATTLE_EFFECT_CAMERA_MODE_SHAKE_DECAYING, 0, 9, &offset);
            }
        }
        done = state->done;
        result = done != 1;
        state->frame++;
        if (state->count_a == 0 && state->count_b == 0 && state->frame >= 31) {
            state->done = done + 1;
        }
        break;
    }
    return result;
}
