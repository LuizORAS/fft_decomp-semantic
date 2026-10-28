#include "fft/battle_effect.h"
#include "fft/main.h"
#include "psx/libgte.h"
#include "psx/suzuki.h"

/* Secondary effect function 0x0c (the trap, BATTLE_EFFECT_SECONDARY_ANIMATION_TRAP) in
 * g_battle_effect_secondary_handlers. It shares the golem handler's shape: initialize and play sound 0x22,
 * then step the effect slots with a gravity of (0, ONE, 0) until finish_timer reaches 1. Returns 1 to keep
 * animating. */
s32 battle_effect_trap_secondary_handler(void) {
    battle_effect_secondary_data_t* effect;
    s32 unused[6]; /* Reserves the target's 0x48-byte frame. */
    battle_effect_gravity_t gravity;
    s32 result;
    s16 finish_timer;

    effect = g_battle_effect_current_secondary;
    /* Preserve the target's unsigned phase-dispatch comparison. */
    switch (effect->phase) {
    case BATTLE_EFFECT_SECONDARY_PHASE_INITIALIZING:
        effect->active_count = 0;
        effect->timer = 0;
        effect->finish_timer = 0;
        effect->phase = BATTLE_EFFECT_SECONDARY_PHASE_EXECUTING;
        suzuki_sound_play_sfx(MAIN_SFX_VENOM_TRAP);
        result = 1;
        break;
    case BATTLE_EFFECT_SECONDARY_PHASE_EXECUTING:
        battle_effect_start_group(BATTLE_EFFECT_PARTICLE_GROUP_TRAP, 12);
        /* fallthrough */
    case BATTLE_EFFECT_SECONDARY_PHASE_FINALIZING:
        battle_effect_save_inertia_threshold();
        battle_effect_set_inertia_threshold(BATTLE_EFFECT_SECONDARY_INERTIA_THRESHOLD);
        gravity.z = 0;
        gravity.x = 0;
        gravity.y = ONE;
        battle_effect_save_gravity();
        battle_effect_set_gravity(&gravity);
        battle_effect_update_active_slots(BATTLE_EFFECT_PARTICLE_GROUP_TRAP);
        battle_effect_restore_inertia_threshold();
        battle_effect_restore_gravity();
        effect = g_battle_effect_current_secondary;
        finish_timer = effect->finish_timer;
        result = finish_timer != 1;
        effect->timer = effect->timer + 1;
        if (effect->active_count == 0
            && effect->timer > g_battle_effect_particle_groups[BATTLE_EFFECT_PARTICLE_GROUP_TRAP].spawn_start_frame) {
            effect->finish_timer = finish_timer + 1;
        }
        break;
    }
    return result;
}
