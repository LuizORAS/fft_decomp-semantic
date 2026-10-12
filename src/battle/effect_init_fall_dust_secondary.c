#include "fft/battle_effect.h"

/* Start the current effect target's fall-dust secondary effect at the
 * given position and return its id. Phase 1 is
 * EFFECT_SECONDARY_PHASE_INITIALIZING. */
s32 effect_init_fall_dust_secondary(effect_secondary_init_t* source, VECTOR* position) {
    s32 secondary_id;

    secondary_id = effect_allocate_secondary_slot();
    g_effect_secondary_data[secondary_id].timer = 0;
    g_effect_secondary_data[secondary_id].animation_id = EFFECT_SECONDARY_ANIMATION_FALL_DUST;
    g_effect_secondary_data[secondary_id].function_id
        = g_effect_secondary_animations[EFFECT_SECONDARY_ANIMATION_FALL_DUST]
              .function_id; /* array access keeps the load after the animation store */
    g_effect_secondary_data[secondary_id].caster = source->targets[EFFECT_CASTER_ENTRY];
    g_effect_secondary_data[secondary_id].target.position.x = position->vx >> 4;
    g_effect_secondary_data[secondary_id].target.position.z = position->vy >> 4;
    g_effect_secondary_data[secondary_id].target.position.y = position->vz >> 4;
    g_effect_secondary_data[secondary_id].phase = EFFECT_SECONDARY_PHASE_INITIALIZING;
    return secondary_id;
}
