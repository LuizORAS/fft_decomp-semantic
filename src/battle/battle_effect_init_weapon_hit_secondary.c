#include "fft/battle.h"
#include "psx/types.h"

/* Start the current effect target's weapon-hit secondary effect.
 *
 * The target's hit result selects the WEAPON_HIT animation (hit, critical,
 * missed, no effect), EQUIP_BREAK (both BREAK results) or GOLEM; REFLECTED
 * keeps the slot's animation. The parameter
 * is the element effect index for the weapon's element mask. Returns its id,
 * or 0 when the init record has no targets. */
s32 battle_effect_init_weapon_hit_secondary(s32 elements, battle_effect_secondary_init_t* source, s32 unused_is_bow) {
    u8 secondary_id;

    if (source->target_count != 0) {
        elements = battle_effect_get_ninja_ball(elements);
        secondary_id = battle_effect_allocate_secondary_slot();
        switch (source->target[0].fields.result_animation) {
        case BATTLE_EFFECT_HIT_NORMAL:
        case BATTLE_EFFECT_HIT_CRITICAL:
        case BATTLE_EFFECT_HIT_MISSED:
        case BATTLE_EFFECT_HIT_NO_EFFECT:
            g_battle_effect_secondary_data[secondary_id].animation_id = BATTLE_EFFECT_SECONDARY_ANIMATION_WEAPON_HIT;
            break;
        case BATTLE_EFFECT_HIT_BREAK:
        case BATTLE_EFFECT_HIT_BREAK_ALT:
            g_battle_effect_secondary_data[secondary_id].animation_id = BATTLE_EFFECT_SECONDARY_ANIMATION_EQUIP_BREAK;
            break;
        case BATTLE_EFFECT_HIT_GOLEM:
            g_battle_effect_secondary_data[secondary_id].animation_id = BATTLE_EFFECT_SECONDARY_ANIMATION_GOLEM;
            break;
        }
        g_battle_effect_secondary_data[secondary_id].parameter = elements;
        g_battle_effect_secondary_data[secondary_id].function_id
            = g_battle_effect_secondary_animations[g_battle_effect_secondary_data[secondary_id].animation_id]
                  .function_id;
        g_battle_effect_secondary_data[secondary_id].caster.block = source->caster.block;
        g_battle_effect_secondary_data[secondary_id].target.block = source->target[0].block;
        g_battle_effect_secondary_data[secondary_id].timer = 0;
        g_battle_effect_secondary_data[secondary_id].phase = BATTLE_SECONDARY_EFFECT_INITIALIZING;
    } else {
        secondary_id = 0;
    }
    return secondary_id;
}
