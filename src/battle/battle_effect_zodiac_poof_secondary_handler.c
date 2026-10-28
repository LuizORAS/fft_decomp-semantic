#include "fft/battle_effect.h"
#include "fft/battle_map.h"
#include "psx/libgte.h"
#include "psx/types.h"

enum {
    BATTLE_EFFECT_TRAP3_SLOT_COUNT = 24,
};

/* Secondary effect function 0x11 (Zodiac Poof): the 24-slot particle group 3.
 *
 * Its spawn state is fixed at 0x801bade0 and its CLUT comes from the table
 * indexed by the effect's parameter, the caster's sprite palette. It follows
 * the same phase structure as the death-poof secondary handler.
 */
s32 battle_effect_zodiac_poof_secondary_handler(void) {
    battle_effect_secondary_data_t* effect;
    battle_effect_zodiac_poof_state_t* state;
    battle_effect_slot_t* slot;
    s32 unused[6]; /* Reserves the target's 0x58-byte frame. */
    battle_effect_gravity_t gravity;
    s32 result;
    s32 spawned;
    s32 i;
    s32 done;
    s16 timer;
    u8 id;
    u16 clut_index;

    effect = g_battle_effect_current_secondary;
    state = &g_battle_effect_secondary_state.zodiac_poof;
    /* Preserve the target's unsigned phase-dispatch comparison. */
    switch (effect->phase) {
    case BATTLE_EFFECT_SECONDARY_PHASE_INITIALIZING:
        state->count = 0;
        for (spawned = 0; spawned < BATTLE_EFFECT_TRAP3_SLOT_COUNT; spawned++) {
            state->ids[spawned] = 0;
        }
        state->done = 0;
        g_battle_effect_current_secondary->timer = 0;
        g_battle_effect_current_secondary->phase = BATTLE_EFFECT_SECONDARY_PHASE_EXECUTING;
        result = 1;
        break;
    case BATTLE_EFFECT_SECONDARY_PHASE_EXECUTING:
        clut_index = g_battle_effect_zodiac_poof_animations[effect->parameter];
        if (effect->timer >= g_battle_effect_particle_groups[BATTLE_EFFECT_PARTICLE_GROUP_ZODIAC_POOF].spawn_start_frame
            && effect->timer
                < g_battle_effect_particle_groups[BATTLE_EFFECT_PARTICLE_GROUP_ZODIAC_POOF].spawn_end_frame) {
            for (spawned = 0;
                spawned < g_battle_effect_particle_groups[BATTLE_EFFECT_PARTICLE_GROUP_ZODIAC_POOF].spawns_per_frame;
                spawned++) {
                for (i = 0; i < BATTLE_EFFECT_TRAP3_SLOT_COUNT; i++) {
                    if (state->ids[i] == 0) {
                        break;
                    }
                }
                id = battle_effect_alloc_slot();
                state->ids[i] = id;
                if (state->ids[i] == 0) {
                    break;
                }
                slot = &g_battle_effect_slots[state->ids[i]];
                slot->life = battle_effect_spawn_particle_motion(
                    &g_battle_effect_particle_groups[BATTLE_EFFECT_PARTICLE_GROUP_ZODIAC_POOF], &slot->motion);
                battle_effect_init_slot_animation(
                    BATTLE_EFFECT_PARTICLE_GROUP_ZODIAC_POOF, clut_index + MAP_CLUT(MAP_CLUT_ROW_WEAPONS, 0), slot);
                state->count++;
                if (state->count == BATTLE_EFFECT_TRAP3_SLOT_COUNT) {
                    break;
                }
            }
        }
        /* fallthrough */
    case BATTLE_EFFECT_SECONDARY_PHASE_FINALIZING:
        battle_effect_save_inertia_threshold();
        battle_effect_set_inertia_threshold(BATTLE_EFFECT_SECONDARY_INERTIA_THRESHOLD);
        gravity.z = 0;
        gravity.x = 0;
        gravity.y = ONE;
        battle_effect_save_gravity();
        battle_effect_set_gravity(&gravity);
        for (spawned = 0; spawned < BATTLE_EFFECT_TRAP3_SLOT_COUNT; spawned++) {
            id = state->ids[spawned];
            if (id != 0 && battle_effect_update_slot(id) == 0) {
                battle_effect_free_slot(id);
                state->ids[spawned] = 0;
                state->count--;
            }
        }
        battle_effect_restore_inertia_threshold();
        battle_effect_restore_gravity();
        {
            battle_effect_secondary_data_t* current_effect;

            current_effect = g_battle_effect_current_secondary;
            timer = current_effect->timer + 1;
            current_effect->timer = timer;
        }
        done = state->done;
        result = done != 1;
        if (state->count == 0
            && timer > g_battle_effect_particle_groups[BATTLE_EFFECT_PARTICLE_GROUP_ZODIAC_POOF].spawn_start_frame) {
            state->done = done + 1;
        }
        break;
    }
    return result;
}
