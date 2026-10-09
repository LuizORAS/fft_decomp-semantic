#include "fft/battle.h"
#include "psx/types.h"

enum {
    BATTLE_EFFECT_TRAP3_SLOT_COUNT = 24,
};

/* Updates the 24-slot secondary trap effect.
 *
 * Its spawn state is fixed at 0x801bade0 and its animation id comes from the
 * per-parameter table at 0x801b88dc. It follows the same phase structure as
 * the death-poof secondary handler while using the trap group's slot records.
 */
s32 battle_effect_zodiac_poof_secondary_handler(void) {
    battle_effect_secondary_data_t* effect;
    battle_effect_trap_state_24_t* state;
    battle_effect_slot_t* slot;
    s32 unused[6]; /* Reserves the target's 0x58-byte frame. */
    battle_effect_gravity_t gravity;
    s32 result;
    s32 spawned;
    s32 i;
    s32 done;
    s16 timer;
    u8 id;
    u16 animation;

    effect = g_battle_effect_current_secondary;
    state = &g_battle_effect_secondary_state.zodiac_poof;
    /* Preserve the target's unsigned phase-dispatch comparison. */
    switch (effect->phase) {
    case BATTLE_SECONDARY_EFFECT_INITIALIZING:
        state->count = 0;
        for (spawned = 0; spawned < BATTLE_EFFECT_TRAP3_SLOT_COUNT; spawned++) {
            state->ids[spawned] = 0;
        }
        state->done = 0;
        g_battle_effect_current_secondary->timer = 0;
        g_battle_effect_current_secondary->phase = BATTLE_SECONDARY_EFFECT_EXECUTING;
        result = 1;
        break;
    case BATTLE_SECONDARY_EFFECT_EXECUTING:
        animation = g_battle_effect_zodiac_poof_animations[effect->parameter];
        if (effect->timer
                >= g_battle_effect_trap_particle_configs[BATTLE_EFFECT_PARTICLE_GROUP_ZODIAC_POOF].spawn_start_frame
            && effect->timer
                < g_battle_effect_trap_particle_configs[BATTLE_EFFECT_PARTICLE_GROUP_ZODIAC_POOF].spawn_end_frame) {
            for (spawned = 0; spawned
                < g_battle_effect_trap_particle_configs[BATTLE_EFFECT_PARTICLE_GROUP_ZODIAC_POOF].spawns_per_frame;
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
                    &g_battle_effect_trap_particle_configs[BATTLE_EFFECT_PARTICLE_GROUP_ZODIAC_POOF], &slot->motion);
                battle_effect_init_trap_animation(BATTLE_EFFECT_PARTICLE_GROUP_ZODIAC_POOF, animation + 0x7ac0, slot);
                state->count++;
                if (state->count == BATTLE_EFFECT_TRAP3_SLOT_COUNT) {
                    break;
                }
            }
        }
        /* fallthrough */
    case BATTLE_SECONDARY_EFFECT_FINALIZING:
        battle_effect_save_inertia_threshold();
        battle_effect_set_inertia_threshold(0x230);
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
            && timer
                > g_battle_effect_trap_particle_configs[BATTLE_EFFECT_PARTICLE_GROUP_ZODIAC_POOF].spawn_start_frame) {
            state->done = done + 1;
        }
        break;
    }
    return result;
}
