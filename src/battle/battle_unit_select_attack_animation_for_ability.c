#include "fft/battle.h"
#include "psx/types.h"

/* Start the attacker's animation for its ability's strike (battle_action_apply_strike_results): the
 * Lucavi and Altima (BATTLE_GFX_SEQ_ID_RUKA to _KANZEN) always use animation 0x2c; any other unit
 * the ability's attack animation (g_battle_ability_animation_data): the weapon's swing
 * (battle_unit_select_weapon_attack_animation), the item's use
 * (battle_unit_set_animation_for_item_abilities) or that animation. */
void battle_unit_select_attack_animation_for_ability(
    battle_unit_misc_data_t* attacker, battle_unit_misc_data_t* target) {
    s32 animation;
    s32 sequence_id;

    sequence_id = g_battle_gfx_spritesheet_data[attacker->spritesheet_id].seq_id;
    if (sequence_id < BATTLE_GFX_SEQ_ID_COUNT) {
        if (sequence_id >= BATTLE_GFX_SEQ_ID_RUKA) {
            battle_unit_store_animation_facing(0x2c, attacker->facing, attacker);
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
