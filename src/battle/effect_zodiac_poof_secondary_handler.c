#include "fft/battle_effect.h"
#include "fft/battle_map.h"
#include "psx/libgte.h"
#include "psx/types.h"

enum {
    EFFECT_TRAP3_SLOT_COUNT = 24,
};

/* Secondary effect function 0x11 (Zodiac Poof): the 24-slot particle group 3.
 *
 * Its spawn state is fixed at 0x801bade0 and its CLUT comes from the table
 * indexed by the effect's parameter, the caster's sprite palette. It follows
 * the same phase structure as the death-poof secondary handler.
 */
s32 effect_zodiac_poof_secondary_handler(void) {
    effect_secondary_data_t* effect;
    effect_trap_state_24_t* state;
    effect_slot_t* slot;
    s32 unused[6]; /* Reserves the target's 0x58-byte frame. */
    vec3s32_t gravity;
    s32 result;
    s32 spawned;
    s32 i;
    s32 done;
    s16 timer;
    u8 id;
    u16 clut_index;

    effect = g_effect_current_secondary;
    state = &g_effect_secondary_state.zodiac_poof;
    /* Preserve the target's unsigned phase-dispatch comparison. */
    switch (effect->phase) {
    case EFFECT_SECONDARY_PHASE_INITIALIZING:
        state->count = 0;
        for (spawned = 0; spawned < EFFECT_TRAP3_SLOT_COUNT; spawned++) {
            state->ids[spawned] = 0;
        }
        state->done = 0;
        g_effect_current_secondary->timer = 0;
        g_effect_current_secondary->phase = EFFECT_SECONDARY_PHASE_EXECUTING;
        result = 1;
        break;
    case EFFECT_SECONDARY_PHASE_EXECUTING:
        clut_index = g_effect_zodiac_poof_animations[effect->parameter];
        if (effect->timer >= g_effect_trap_particle_configs[EFFECT_PARTICLE_GROUP_ZODIAC_POOF].spawn_start_frame
            && effect->timer < g_effect_trap_particle_configs[EFFECT_PARTICLE_GROUP_ZODIAC_POOF].spawn_end_frame) {
            for (spawned = 0;
                spawned < g_effect_trap_particle_configs[EFFECT_PARTICLE_GROUP_ZODIAC_POOF].spawns_per_frame;
                spawned++) {
                for (i = 0; i < EFFECT_TRAP3_SLOT_COUNT; i++) {
                    if (state->ids[i] == 0) {
                        break;
                    }
                }
                id = effect_alloc_slot();
                state->ids[i] = id;
                if (state->ids[i] == 0) {
                    break;
                }
                slot = &g_effect_slots[state->ids[i]];
                slot->life = effect_spawn_particle_motion(
                    &g_effect_trap_particle_configs[EFFECT_PARTICLE_GROUP_ZODIAC_POOF], &slot->motion);
                effect_init_trap_animation(
                    EFFECT_PARTICLE_GROUP_ZODIAC_POOF, clut_index + MAP_CLUT(MAP_CLUT_ROW_WEAPONS, 0), slot);
                state->count++;
                if (state->count == EFFECT_TRAP3_SLOT_COUNT) {
                    break;
                }
            }
        }
        /* fallthrough */
    case EFFECT_SECONDARY_PHASE_FINALIZING:
        effect_save_inertia_threshold();
        effect_set_inertia_threshold(EFFECT_SECONDARY_INERTIA_THRESHOLD);
        gravity.z = 0;
        gravity.x = 0;
        gravity.y = ONE;
        effect_save_gravity();
        effect_set_gravity(&gravity);
        for (spawned = 0; spawned < EFFECT_TRAP3_SLOT_COUNT; spawned++) {
            id = state->ids[spawned];
            if (id != 0 && effect_update_slot(id) == 0) {
                effect_free_slot(id);
                state->ids[spawned] = 0;
                state->count--;
            }
        }
        effect_restore_inertia_threshold();
        effect_restore_gravity();
        {
            effect_secondary_data_t* current_effect;

            current_effect = g_effect_current_secondary;
            timer = current_effect->timer + 1;
            current_effect->timer = timer;
        }
        done = state->done;
        result = done != 1;
        if (state->count == 0
            && timer > g_effect_trap_particle_configs[EFFECT_PARTICLE_GROUP_ZODIAC_POOF].spawn_start_frame) {
            state->done = done + 1;
        }
        break;
    }
    return result;
}
