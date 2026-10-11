#include "fft/battle.h"

/* Start secondary effect animation_id for the caster and first target of the
 * init record and return its id. parameter is the animation's own: an
 * element mask, a sprite palette or a duration. */
s32 battle_effect_init_secondary(s32 animation_id, s32 parameter, battle_effect_secondary_init_t* source) {
    s32 secondary_id;

    secondary_id = battle_effect_allocate_secondary_slot();
    g_battle_effect_secondary_data[secondary_id].animation_id = animation_id;
    g_battle_effect_secondary_data[secondary_id].timer = 0;
    g_battle_effect_secondary_data[secondary_id].parameter = parameter;
    g_battle_effect_secondary_data[secondary_id].function_id
        = g_battle_effect_secondary_animations[g_battle_effect_secondary_data[secondary_id].animation_id].function_id;
    g_battle_effect_secondary_data[secondary_id].caster = source->targets[BATTLE_EFFECT_CASTER_ENTRY];
    g_battle_effect_secondary_data[secondary_id].target.record = source->targets[0];
    g_battle_effect_secondary_data[secondary_id].used_weapon_id = source->used_weapon_id;
    g_battle_effect_secondary_data[secondary_id].phase = BATTLE_SECONDARY_EFFECT_INITIALIZING;
    return secondary_id;
}
