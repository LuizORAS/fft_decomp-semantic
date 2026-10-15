#include "fft/battle.h"
#include "psx/types.h"

/* Start the pre-attack variant of the ability's charge animation set
 * (g_battle_ability_charge_animation_sets, chosen by g_battle_ability_animation_data), if the set has
 * one; battle_state_enter_pre_attack_animation calls it for an ability with a charge set. */
void battle_unit_start_ability_pre_attack_animation(battle_unit_misc_data_t* unit) {
    u8 animation;

    animation = g_battle_ability_charge_animation_sets[g_battle_ability_animation_data[unit->used_ability_id]
            .charge_animation_set_id][BATTLE_CHARGE_ANIMATION_PRE_ATTACK];
    if (animation != 0) {
        battle_unit_store_animation_facing(animation, unit->facing, unit);
    }
}
