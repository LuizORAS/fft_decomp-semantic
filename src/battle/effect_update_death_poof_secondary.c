#include "fft/battle_effect.h"
#include "fft/battle_map.h"
#include "psx/libgte.h"
#include "psx/types.h"

enum {
    EFFECT_TRAP_SLOT_COUNT = 2,
    EFFECT_TRAP_CLUT = MAP_CLUT(MAP_CLUT_ROW_WEAPONS, 15),
};

/*
 * Death Poof secondary effect (0x0f) handler for the two-slot trap group 4: it
 * starts this frame's share of the group's effect slots, steps them with a
 * gravity of (0, ONE, 0) and finishes once the spawn state's finish_timer
 * reaches 1.
 *
 * The handler table calls it with no arguments: for a phase outside 1-3 the
 * result is `passthrough`, whatever the caller left in the second argument
 * register.
 */
s32 effect_update_death_poof_secondary(s32 unused_arg0, s32 passthrough) {
    effect_secondary_data_t* effect;
    effect_death_poof_state_t* state;
    s32 unused[6]; /* Reserves the target's 0x58-byte frame. */
    vec3s32_t gravity;
    s32 result;
    s32 spawned;
    s32 i;

    effect = g_effect_current_secondary;
    result = passthrough;
    state = &g_effect_death_poof_states[effect->own_slot_id];
    /* Preserve the target's unsigned phase-dispatch comparison. */
    switch (effect->phase) {
    case EFFECT_SECONDARY_PHASE_INITIALIZING: {
        state->count = 0;
        for (i = 0; i < EFFECT_TRAP_SLOT_COUNT; i++) {
            state->ids[i] = 0;
        }
        state->finish_timer = 0;
        g_effect_current_secondary->timer = 0;
        g_effect_current_secondary->phase = EFFECT_SECONDARY_PHASE_EXECUTING;
        result = 1;
        break;
    }
    case EFFECT_SECONDARY_PHASE_EXECUTING: {
        effect_slot_t* slot;
        s32 i;
        u8 id;

        if (effect->timer >= g_effect_trap_particle_configs[EFFECT_PARTICLE_GROUP_DEATH_POOF].spawn_start_frame
            && effect->timer < g_effect_trap_particle_configs[EFFECT_PARTICLE_GROUP_DEATH_POOF].spawn_end_frame) {
            for (spawned = 0;
                spawned < g_effect_trap_particle_configs[EFFECT_PARTICLE_GROUP_DEATH_POOF].spawns_per_frame;
                spawned++) {
                for (i = 0; i < EFFECT_TRAP_SLOT_COUNT; i++) {
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
                    &g_effect_trap_particle_configs[EFFECT_PARTICLE_GROUP_DEATH_POOF], &slot->motion);
                effect_init_trap_animation(EFFECT_PARTICLE_GROUP_DEATH_POOF, EFFECT_TRAP_CLUT, slot);
                state->count++;
                if (state->count == EFFECT_TRAP_SLOT_COUNT) {
                    break;
                }
            }
        }
    }
    /* fallthrough */
    case EFFECT_SECONDARY_PHASE_FINALIZING: {
        effect_secondary_data_t* current;
        u8 id;

        effect_save_inertia_threshold();
        effect_set_inertia_threshold(EFFECT_SECONDARY_INERTIA_THRESHOLD);
        gravity.z = 0;
        gravity.x = 0;
        gravity.y = ONE;
        effect_save_gravity();
        effect_set_gravity(&gravity);
        for (i = 0; i < EFFECT_TRAP_SLOT_COUNT; i++) {
            id = state->ids[i];
            if (id != 0 && effect_update_slot(id) == 0) {
                effect_free_slot(id);
                state->ids[i] = 0;
                state->count--;
            }
        }
        effect_restore_inertia_threshold();
        effect_restore_gravity();
        current = g_effect_current_secondary;
        current->timer = current->timer + 1;
        result = state->finish_timer != 1;
        if (state->count == 0
            && current->timer > g_effect_trap_particle_configs[EFFECT_PARTICLE_GROUP_DEATH_POOF].spawn_start_frame) {
            state->finish_timer = state->finish_timer + 1;
        }
        break;
    }
    }
    return result;
}
