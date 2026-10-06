#include "fft/battle.h"
#include "psx/types.h"

/* Pick the piece of equipment a Steal, Break or Might Sword ability takes: helmet (0x6e/0x8a/0xa1),
 * armor (0x6f/0x8b/0xa0), shield (0x70/0x8c: the right hand's, else the left's), accessory
 * (0x72/0xa3) or weapon (0x71/0x8d/0xa2: the hand whose item needs the higher level, right on ties).
 * It records the slot bit in equipment_destroyed and the item in item_lost and returns 0. Monsters,
 * empty slots and other abilities clear equipment_destroyed and return -1: Steal and Might Sword then
 * fail, and Break becomes a plain Attack (battle_action_switch_ability_to_default_attack). */
s32 battle_formula_select_target_equipment(void) {
    u16 ability;
    s32 right_level;
    s32 left_level;

    g_battle_action_target_data->equipment_destroyed = 0xfe;
    if (!(g_battle_action_target->unit_flags & UNIT_FLAG_MONSTER)) {
        ability = g_current_ability.ability_id;
        if (ability == ABILITY_ID_BATTLE_SKILL_HEAD_BREAK || ability == ABILITY_ID_MIGHT_SWORD_BLASTAR_PUNCH
            || ability == ABILITY_ID_STEAL_HELMET) {
            if (g_battle_action_target->equipment[UNIT_EQUIPMENT_SLOT_HEAD] != ITEM_ID_NONE) {
                g_battle_action_target_data->equipment_destroyed = BATTLE_ACTION_EQUIPMENT_SLOT_HEAD;
                g_battle_action_target_data->item_lost = g_battle_action_target->equipment[UNIT_EQUIPMENT_SLOT_HEAD];
                return 0;
            }
        } else if (ability == ABILITY_ID_BATTLE_SKILL_ARMOR_BREAK || ability == ABILITY_ID_MIGHT_SWORD_SHELLBUST_STAB
            || ability == ABILITY_ID_STEAL_ARMOR) {
            if (g_battle_action_target->equipment[UNIT_EQUIPMENT_SLOT_BODY] != ITEM_ID_NONE) {
                g_battle_action_target_data->equipment_destroyed = BATTLE_ACTION_EQUIPMENT_SLOT_BODY;
                g_battle_action_target_data->item_lost = g_battle_action_target->equipment[UNIT_EQUIPMENT_SLOT_BODY];
                return 0;
            }
        } else if (ability == ABILITY_ID_BATTLE_SKILL_SHIELD_BREAK || ability == ABILITY_ID_STEAL_SHIELD) {
            if (g_battle_action_target->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_SHIELD] != ITEM_ID_NONE) {
                g_battle_action_target_data->equipment_destroyed = BATTLE_ACTION_EQUIPMENT_SLOT_RIGHT_SHIELD;
                g_battle_action_target_data->item_lost
                    = g_battle_action_target->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_SHIELD];
                return 0;
            }
            if (g_battle_action_target->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_SHIELD] != ITEM_ID_NONE) {
                g_battle_action_target_data->equipment_destroyed = BATTLE_ACTION_EQUIPMENT_SLOT_LEFT_SHIELD;
                g_battle_action_target_data->item_lost
                    = g_battle_action_target->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_SHIELD];
                return 0;
            }
        } else if (ability == ABILITY_ID_MIGHT_SWORD_ICEWOLF_BITE || ability == ABILITY_ID_STEAL_ACCESSORY) {
            if (g_battle_action_target->equipment[UNIT_EQUIPMENT_SLOT_ACCESSORY] != ITEM_ID_NONE) {
                g_battle_action_target_data->equipment_destroyed = BATTLE_ACTION_EQUIPMENT_SLOT_ACCESSORY;
                g_battle_action_target_data->item_lost
                    = g_battle_action_target->equipment[UNIT_EQUIPMENT_SLOT_ACCESSORY];
                return 0;
            }
        } else if (ability == ABILITY_ID_BATTLE_SKILL_WEAPON_BREAK || ability == ABILITY_ID_MIGHT_SWORD_HELLCRY_PUNCH
            || ability == ABILITY_ID_STEAL_WEAPON) {
            left_level = -1;
            right_level = -1;
            if (g_battle_action_target->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON] != ITEM_ID_NONE) {
                right_level
                    = g_main_item_primary_data[g_battle_action_target->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON]]
                          .required_level;
            }
            if (g_battle_action_target->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON] != ITEM_ID_NONE) {
                left_level
                    = g_main_item_primary_data[g_battle_action_target->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON]]
                          .required_level;
            }
            if (right_level != -1 || left_level != right_level) {
                if (right_level >= left_level) {
                    g_battle_action_target_data->equipment_destroyed = BATTLE_ACTION_EQUIPMENT_SLOT_RIGHT_WEAPON;
                    g_battle_action_target_data->item_lost
                        = g_battle_action_target->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON];
                    return 0;
                }
                g_battle_action_target_data->equipment_destroyed = BATTLE_ACTION_EQUIPMENT_SLOT_LEFT_WEAPON;
                g_battle_action_target_data->item_lost
                    = g_battle_action_target->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON];
                return 0;
            }
        }
    }
    g_battle_action_target_data->equipment_destroyed = 0;
    return -1;
}
