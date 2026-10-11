#include "fft/battle.h"
#include "psx/types.h"

/* Start the attacker's animation for its ability's strike (battle_action_apply_strike_results): the
 * Lucavi and Altima (BATTLE_SEQ_RUKA to _KANZEN) always use animation 0x2c; any other unit
 * the ability's attack animation (g_battle_ability_animation_data): the weapon's swing
 * (battle_unit_select_weapon_attack_animation), the item's use
 * (battle_unit_set_animation_for_item_abilities) or that animation. */
void battle_unit_select_attack_animation_for_ability(
    battle_unit_misc_data_t* attacker, battle_unit_misc_data_t* target) {
    s32 animation;
    s32 seq_type;

    seq_type = g_battle_gfx_spritesheet_data[attacker->spritesheet_id].seq_id;
    if (seq_type < 8) {
        if (seq_type >= BATTLE_SEQ_RUKA) {
            /* facing is loaded signed (lh) here. */
            battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_CAST, attacker->facing, attacker);
            return;
        }
    }

    animation = g_battle_ability_animation_data[attacker->used_ability_id].attack_animation;
    if (animation == BATTLE_ATTACK_ANIMATION_USE_WEAPON) {
        battle_unit_select_weapon_attack_animation(attacker, target);
        return;
    }
    if (animation == BATTLE_ATTACK_ANIMATION_USE_ITEM) {
        battle_unit_set_animation_for_item_abilities(attacker, target);
        return;
    }
    battle_unit_store_animation_facing(animation, attacker->facing, attacker);
}
