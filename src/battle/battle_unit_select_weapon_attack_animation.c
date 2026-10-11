#include "fft/battle.h"
#include "psx/types.h"

/* Choose the attacker's weapon attack animation from the target's height.
 *
 * g_battle_weapon_attack_animations has three animations for each item type:
 * at a higher target, a level one and a lower one. A bow indexes its row by
 * battle_effect_get_bow_shot_angle. Other weapons compare the vertical
 * centres of the two sprites: a target 12 or more above the attacker takes
 * the first animation and one 12 or more below the third. A Golem or Blade
 * Grasp guard always takes the first, and the sheets from the crystal up
 * always play the unarmed level attack. */
void battle_unit_select_weapon_attack_animation(battle_unit_misc_data_t* attacker, battle_unit_misc_data_t* target) {
    s32 miss_type;
    s16 attacker_centre;
    s16 target_centre;
    s32 difference;

    if (attacker == 0) {
        return;
    }
    if (target != 0) {
        if (attacker->spritesheet_id >= BATTLE_SPRITESHEET_ID_CRYSTAL) {
            battle_unit_store_animation_facing_movement_data(
                g_battle_weapon_attack_animations[0][1], attacker->facing, attacker);
            return;
        }
        if (attacker->equipped_weapon_type == ITEM_TYPE_BOW) {
            battle_unit_store_animation_facing_movement_data(
                g_battle_weapon_attack_animations[ITEM_TYPE_BOW][battle_effect_get_bow_shot_angle()], attacker->facing,
                attacker);
            return;
        }
        miss_type = target->battle_data->action.miss_type;
        if (miss_type != BATTLE_ACTION_MISS_TYPE_HIT) {
            if (miss_type >= BATTLE_ACTION_MISS_TYPE_REFLECTED) {
                if (miss_type < BATTLE_ACTION_MISS_TYPE_BUGGED_REFLECTION) {
                    if (miss_type >= BATTLE_ACTION_MISS_TYPE_GOLEM_GUARD) {
                        battle_unit_store_animation_facing_movement_data(
                            g_battle_weapon_attack_animations[attacker->equipped_weapon_type][0], attacker->facing,
                            attacker);
                        return;
                    }
                }
            }
        }
        /* The target loads the attacker's screen_z zero-extended (lhu). */
        attacker_centre
            = battle_gfx_get_unit_spritesheet_height_by_misc_id(attacker->unit_id) / 2 - attacker->screen.vy;
        target_centre = battle_gfx_get_unit_spritesheet_height_by_misc_id(target->unit_id) / 2 - target->screen.vy;
        difference = attacker_centre - target_centre;
        if (difference < -11) {
            battle_unit_store_animation_facing_movement_data(
                g_battle_weapon_attack_animations[attacker->equipped_weapon_type][0], attacker->facing, attacker);
            return;
        }
        if (difference >= 12) {
            battle_unit_store_animation_facing_movement_data(
                g_battle_weapon_attack_animations[attacker->equipped_weapon_type][2], attacker->facing, attacker);
            return;
        }
    }
    battle_unit_store_animation_facing_movement_data(
        g_battle_weapon_attack_animations[attacker->equipped_weapon_type][1], attacker->facing, attacker);
}
