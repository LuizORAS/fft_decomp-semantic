#include "fft/battle_effect.h"
#include "psx/libgte.h"

/*
 * Secondary effect function 0x15 (Equip Break) in g_effect_secondary_handlers.
 * Same shape as the venom trap
 * handler, but its particle group is held in a variable, so its spawn_start_frame lookup
 * is an indexed table read. Returns 1 to keep animating.
 */
s32 effect_equip_break_secondary_handler(void) {
    effect_secondary_data_t* effect;
    effect_secondary_data_t* current;
    vec3s32_t gravity;
    s32 group;
    s32 result;
    s16 finish_timer;

    group = EFFECT_PARTICLE_GROUP_EQUIP_BREAK;
    effect = g_effect_current_secondary;
    /* Preserve the target's unsigned phase-dispatch comparison. */
    switch (effect->phase) {
    case EFFECT_SECONDARY_PHASE_INITIALIZING:
        result = 1;
        effect->active_count = 0;
        effect->timer = 0;
        effect->finish_timer = 0;
        effect->phase = EFFECT_SECONDARY_PHASE_EXECUTING;
        break;
    case EFFECT_SECONDARY_PHASE_EXECUTING:
        effect_start_group(group, 10);
        /* fallthrough */
    case EFFECT_SECONDARY_PHASE_FINALIZING:
        effect_save_inertia_threshold();
        effect_set_inertia_threshold(EFFECT_SECONDARY_INERTIA_THRESHOLD);
        gravity.z = 0;
        gravity.x = 0;
        gravity.y = ONE;
        effect_save_gravity();
        effect_set_gravity(&gravity);
        effect_update_active_slots(group);
        effect_restore_inertia_threshold();
        effect_restore_gravity();
        current = g_effect_current_secondary;
        finish_timer = current->finish_timer;
        result = finish_timer != 1;
        current->timer = current->timer + 1;
        if (current->active_count == 0 && current->timer > g_effect_trap_particle_configs[group].spawn_start_frame) {
            current->finish_timer = finish_timer + 1;
        }
        break;
    }
    return result;
}
