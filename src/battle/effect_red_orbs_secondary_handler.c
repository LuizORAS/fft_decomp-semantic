#include "fft/battle_effect.h"
#include "fft/main.h"
#include "psx/libgte.h"
#include "psx/suzuki.h"

/*
 * Secondary effect function 0x06 (Red Orbs) in g_effect_secondary_handlers.
 * It shares the golem handler's
 * shape: initialize and play sound 8, then step the effect slots with a
 * gravity of (0, ONE, 0) until finish_timer reaches 1. Returns 1 to keep
 * animating.
 */
s32 effect_red_orbs_secondary_handler(void) {
    effect_secondary_data_t* effect;
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
        suzuki_sound_play_sfx(MAIN_SFX_RED_ORBS);
        result = 1;
        break;
    case EFFECT_SECONDARY_PHASE_EXECUTING:
        effect_start_group(EFFECT_PARTICLE_GROUP_RED_ORBS, 11);
        /* fallthrough */
    case EFFECT_SECONDARY_PHASE_FINALIZING:
        effect_save_inertia_threshold();
        effect_set_inertia_threshold(EFFECT_SECONDARY_INERTIA_THRESHOLD);
        gravity.z = 0;
        gravity.x = 0;
        gravity.y = ONE;
        effect_save_gravity();
        effect_set_gravity(&gravity);
        effect_update_active_slots(EFFECT_PARTICLE_GROUP_RED_ORBS);
        effect_restore_inertia_threshold();
        effect_restore_gravity();
        effect = g_effect_current_secondary;
        finish_timer = effect->finish_timer;
        result = finish_timer != 1;
        effect->timer = effect->timer + 1;
        if (effect->active_count == 0
            && effect->timer > g_effect_trap_particle_configs[EFFECT_PARTICLE_GROUP_RED_ORBS].spawn_start_frame) {
            effect->finish_timer = finish_timer + 1;
        }
        break;
    }
    return result;
}
