#include "fft/battle.h"

/*
 * Secondary effect function 0x13 (Teleport) in g_battle_effect_secondary_handlers.
 * It shares the golem handler's
 * shape: initialize, then step the effect slots with a gravity of
 * (0, ONE, 0) until finish_timer reaches 1. Returns 1 to keep animating.
 */
s32 battle_effect_teleport_secondary_handler(void) {
    battle_effect_secondary_data_t* effect;
    s32 unused[6]; /* Reserves the target's 0x48-byte frame. */
    battle_effect_gravity_t gravity;
    s32 result;
    s16 finish_timer;

    /* Preserve the target's unsigned phase-dispatch comparison. */
    switch (g_battle_effect_current_secondary->phase) {
    case BATTLE_SECONDARY_EFFECT_INITIALIZING:
        result = 1;
        g_battle_effect_current_secondary->active_count = 0;
        g_battle_effect_current_secondary->timer = 0;
        g_battle_effect_current_secondary->finish_timer = 0;
        g_battle_effect_current_secondary->phase = BATTLE_SECONDARY_EFFECT_EXECUTING;
        break;
    case BATTLE_SECONDARY_EFFECT_EXECUTING:
        battle_effect_start_group(BATTLE_EFFECT_PARTICLE_GROUP_TELEPORT, 11);
        /* fallthrough */
    case BATTLE_SECONDARY_EFFECT_FINALIZING:
        battle_effect_save_inertia_threshold();
        battle_effect_set_inertia_threshold(BATTLE_EFFECT_SECONDARY_INERTIA_THRESHOLD);
        gravity.z = 0;
        gravity.x = 0;
        gravity.y = ONE;
        battle_effect_save_gravity();
        battle_effect_set_gravity(&gravity);
        battle_effect_update_active_slots(BATTLE_EFFECT_PARTICLE_GROUP_TELEPORT);
        battle_effect_restore_inertia_threshold();
        battle_effect_restore_gravity();
        effect = g_battle_effect_current_secondary;
        finish_timer = effect->finish_timer;
        result = finish_timer != 1;
        effect->timer = effect->timer + 1;
        if (effect->active_count == 0
            && effect->timer
                > g_battle_effect_trap_particle_configs[BATTLE_EFFECT_PARTICLE_GROUP_TELEPORT].spawn_start_frame) {
            effect->finish_timer = finish_timer + 1;
        }
        break;
    }
    return result;
}
