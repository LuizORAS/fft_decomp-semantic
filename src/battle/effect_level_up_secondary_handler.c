#include "fft/battle_effect.h"
#include "psx/libgte.h"

/*
 * Secondary effect function 0x0d (Level Up) in g_effect_secondary_handlers.
 * It shares the venom trap
 * handler's shape but seeds group 5's position_y with -8 and lowers it by 3 per
 * executing frame instead of playing a sound. Returns 1 to keep animating.
 */
s32 effect_level_up_secondary_handler(void) {
    effect_secondary_data_t* effect;
    effect_secondary_data_t* current;
    s32 unused[6]; /* Reserves the target's 0x48-byte frame. */
    vec3s32_t gravity;
    s32 result;
    s16 finish_timer;

    effect = g_effect_current_secondary;
    /* Preserve the target's unsigned phase-dispatch comparison. */
    switch (effect->phase) {
    case EFFECT_SECONDARY_PHASE_INITIALIZING:
        effect->active_count = 0;
        effect->timer = 0;
        effect->finish_timer = 0;
        effect->phase = EFFECT_SECONDARY_PHASE_EXECUTING;
        g_effect_trap_particle_configs[EFFECT_PARTICLE_GROUP_LEVEL_UP].position.y = -8;
        result = 1;
        break;
    case EFFECT_SECONDARY_PHASE_EXECUTING:
        effect_start_group(EFFECT_PARTICLE_GROUP_LEVEL_UP, 11);
        {
            /* Addressing through a pointer keeps the target lui/addiu pair. */
            s16* rise = &g_effect_trap_particle_configs[EFFECT_PARTICLE_GROUP_LEVEL_UP].position.y;
            *rise -= 3;
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
        effect_update_active_slots(EFFECT_PARTICLE_GROUP_LEVEL_UP);
        effect_restore_inertia_threshold();
        effect_restore_gravity();
        current = g_effect_current_secondary;
        finish_timer = current->finish_timer;
        result = finish_timer != 1;
        current->timer = current->timer + 1;
        if (current->active_count == 0
            && current->timer > g_effect_trap_particle_configs[EFFECT_PARTICLE_GROUP_LEVEL_UP].spawn_start_frame) {
            current->finish_timer = finish_timer + 1;
        }
        break;
    }
    return result;
}
