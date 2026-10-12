#include "fft/battle_effect.h"
#include "psx/types.h"

/* Start the current effect target's weapon-hit secondary effect.
 *
 * The target's hit result selects the WEAPON_HIT animation (hit, critical,
 * missed, no effect), EQUIP_BREAK (both BREAK results) or GOLEM; REFLECTED
 * keeps the slot's animation. The parameter
 * is the element effect index for the weapon's element mask. Returns its id,
 * or 0 when the init record has no targets. */
s32 effect_init_weapon_hit_secondary(s32 elements, effect_secondary_init_t* source, s32 unused_is_bow) {
    u8 secondary_id;

    if (source->target_count != 0) {
        elements = effect_get_ninja_ball(elements);
        secondary_id = effect_allocate_secondary_slot();
        switch (source->targets[0].data.unit.result_animation) {
        case EFFECT_HIT_NORMAL:
        case EFFECT_HIT_CRITICAL:
        case EFFECT_HIT_MISSED:
        case EFFECT_HIT_NO_EFFECT:
            g_effect_secondary_data[secondary_id].animation_id = EFFECT_SECONDARY_ANIMATION_WEAPON_HIT;
            break;
        case EFFECT_HIT_BREAK:
        case EFFECT_HIT_BREAK_ALT:
            g_effect_secondary_data[secondary_id].animation_id = EFFECT_SECONDARY_ANIMATION_EQUIP_BREAK;
            break;
        case EFFECT_HIT_GOLEM:
            g_effect_secondary_data[secondary_id].animation_id = EFFECT_SECONDARY_ANIMATION_GOLEM;
            break;
        }
        g_effect_secondary_data[secondary_id].parameter = elements;
        g_effect_secondary_data[secondary_id].function_id
            = g_effect_secondary_animations[g_effect_secondary_data[secondary_id].animation_id].function_id;
        g_effect_secondary_data[secondary_id].caster = source->targets[EFFECT_CASTER_ENTRY];
        g_effect_secondary_data[secondary_id].target.record = source->targets[0];
        g_effect_secondary_data[secondary_id].timer = 0;
        g_effect_secondary_data[secondary_id].phase = EFFECT_SECONDARY_PHASE_INITIALIZING;
    } else {
        secondary_id = 0;
    }
    return secondary_id;
}
