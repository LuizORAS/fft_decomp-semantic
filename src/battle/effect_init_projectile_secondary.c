#include "fft/battle_effect.h"

/* Start the current effect target's secondary effect: the low half of
 * animation selects the animation id (and its function id), the high half is
 * stored alongside it. Returns its id; phase 1 is
 * EFFECT_SECONDARY_PHASE_INITIALIZING. */
s32 effect_init_projectile_secondary(s32 animation, effect_secondary_init_t* source) {
    u8 secondary_id;

    secondary_id = effect_allocate_secondary_slot();
    g_effect_secondary_data[secondary_id].animation_id = animation;
    g_effect_secondary_data[secondary_id].timer = 0;
    g_effect_secondary_data[secondary_id].parameter = animation >> 16;
    g_effect_secondary_data[secondary_id].function_id
        = g_effect_secondary_animations[g_effect_secondary_data[secondary_id].animation_id].function_id;
    g_effect_secondary_data[secondary_id].caster = source->targets[EFFECT_CASTER_ENTRY];
    g_effect_secondary_data[secondary_id].target.record = source->targets[0];
    g_effect_secondary_data[secondary_id].used_weapon_id = source->used_weapon_id;
    g_effect_secondary_data[secondary_id].phase = EFFECT_SECONDARY_PHASE_INITIALIZING;
    g_effect_secondary_data[secondary_id].target_count = source->target_count;
    return secondary_id;
}
