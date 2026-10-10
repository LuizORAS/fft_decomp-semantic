#include "fft/battle.h"

/* Start the current effect target's fall-dust secondary effect at the
 * given position and return its id. Phase 1 is
 * BATTLE_SECONDARY_EFFECT_INITIALIZING. */
s32 battle_effect_init_fall_dust_secondary(battle_effect_secondary_init_t* source, VECTOR* position) {
    s32 secondary_id;

    secondary_id = battle_effect_allocate_secondary_slot();
    g_battle_effect_secondary_data[secondary_id].timer = 0;
    g_battle_effect_secondary_data[secondary_id].animation_id = BATTLE_EFFECT_SECONDARY_ANIMATION_FALL_DUST;
    g_battle_effect_secondary_data[secondary_id].function_id
        = g_battle_effect_secondary_animations[BATTLE_EFFECT_SECONDARY_ANIMATION_FALL_DUST]
              .function_id; /* array access keeps the load after the animation store */
    g_battle_effect_secondary_data[secondary_id].caster.block = source->caster.block;
    g_battle_effect_secondary_data[secondary_id].target.position.x = position->vx >> 4;
    g_battle_effect_secondary_data[secondary_id].target.position.z = position->vy >> 4;
    g_battle_effect_secondary_data[secondary_id].target.position.y = position->vz >> 4;
    g_battle_effect_secondary_data[secondary_id].phase = BATTLE_SECONDARY_EFFECT_INITIALIZING;
    return secondary_id;
}
