#include "fft/battle.h"

s32 battle_effect_golem_secondary_handler(void) {
    battle_effect_secondary_data_t* effect;
    battle_effect_secondary_data_t* current;
    s32 unused[6];
    battle_effect_gravity_t gravity;
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
