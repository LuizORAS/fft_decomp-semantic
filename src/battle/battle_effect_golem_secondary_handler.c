#include "fft/battle_effect.h"

/* Secondary effect function 0x03 in g_battle_effect_secondary_handlers: the
 * particles of BATTLE_EFFECT_SECONDARY_ANIMATION_GOLEM.
 *
 * Initializing clears the counters. Executing starts this frame's share of the
 * Golem particle group with CLUT 13 of the weapons row and, like Finalizing,
 * steps the live slots under the secondary-effect inertia threshold and a gravity
 * of (0, ONE, 0), then puts back the effect file's own values. Once no slot is
 * live and the timer has passed the group's spawn_start_frame, finish_timer
 * counts up; the handler returns 0 on the frame after that begins, and 1 until
 * then. */
s32 battle_effect_golem_secondary_handler(void) {
    battle_effect_secondary_data_t* effect;
    battle_effect_secondary_data_t* current;
    s32 unused[6];
    vec3s32_t gravity;
    s32 result;
    s16 finish_timer;

    effect = g_battle_effect_current_secondary;
    /* Preserve the target's unsigned phase-dispatch comparison. */
    switch (effect->phase) {
    case BATTLE_SECONDARY_EFFECT_INITIALIZING:
        result = 1;
        effect->active_count = 0;
        effect->timer = 0;
        effect->finish_timer = 0;
        effect->phase = BATTLE_SECONDARY_EFFECT_EXECUTING;
        break;
    case BATTLE_SECONDARY_EFFECT_EXECUTING:
        battle_effect_start_group(BATTLE_EFFECT_PARTICLE_GROUP_GOLEM, 0xD);
        /* fallthrough */
    case BATTLE_SECONDARY_EFFECT_FINALIZING:
        battle_effect_save_inertia_threshold();
        battle_effect_set_inertia_threshold(BATTLE_EFFECT_SECONDARY_INERTIA_THRESHOLD);
        gravity.z = 0;
        gravity.x = 0;
        gravity.y = ONE;
        battle_effect_save_gravity();
        battle_effect_set_gravity(&gravity);
        battle_effect_update_active_slots(BATTLE_EFFECT_PARTICLE_GROUP_GOLEM);
        battle_effect_restore_inertia_threshold();
        battle_effect_restore_gravity();
        current = g_battle_effect_current_secondary;
        finish_timer = current->finish_timer;
        result = finish_timer != 1;
        current->timer = current->timer + 1;
        if (current->active_count == 0
            && current->timer
                > g_battle_effect_trap_particle_configs[BATTLE_EFFECT_PARTICLE_GROUP_GOLEM].spawn_start_frame) {
            current->finish_timer = finish_timer + 1;
        }
        break;
    }
    return result;
}
