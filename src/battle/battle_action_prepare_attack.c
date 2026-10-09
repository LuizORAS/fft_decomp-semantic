#include "fft/battle.h"
#include "psx/types.h"

/*
 * The one-trip loop around `skip = 1` counts that set one loop level deeper,
 * so global-alloc ranks `skip` above `persevere` and gives them s6/s7 as in
 * the target.
 */
s32 battle_action_prepare_attack(battle_ai_command_action_t* source, battle_ai_command_action_t* dest, s32 phase) {
    battle_ai_command_action_t action;
    battle_stats_t* unit;
    ability_secondary_data_t* entry;
    s16 ability;
    u8* table;
    u8 jump_ct;
    s32 item;
    u8 ct;
    u8 persevere;
    s32 skip;
    u16 type;
    u16 multiplier;
    u8 support;

    main_util_copy_action_data(source, &action);
    if (action.unit_id >= BATTLE_UNIT_SLOT_COUNT) {
        return BATTLE_ATTACK_PREPARATION_INVALID;
    }
    unit = &g_battle_unit_stats[action.unit_id];
    if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE) {
        return BATTLE_ATTACK_PREPARATION_INVALID;
    }
    skip = 0;
    if (action.targeting_type == 6) {
        if (action.target_id >= BATTLE_UNIT_SLOT_COUNT) {
            return BATTLE_ATTACK_PREPARATION_INVALID;
        }
        if (g_battle_unit_stats[action.target_id].entd_slot == BATTLE_ENTD_SLOT_NONE) {
            return BATTLE_ATTACK_PREPARATION_INVALID;
        }
    }
    switch (g_main_action_menu_types_by_skillset[action.skillset]) {
    case ACTION_MENU_TYPE_ITEM_INVENTORY:
        item = action.item_id;
        if (g_main_item_primary_data[item & 0xff].type != ITEM_TYPE_CONSUMABLE) {
            return BATTLE_ATTACK_PREPARATION_INVALID;
        }
        main_util_copy_action_data(&action, dest);
        ability = item + (ABILITY_ID_ITEM_FIRST - ITEM_ID_CONSUMABLE_FIRST);
        dest->ability_id = ability;
        if (phase != 0 && (phase & 1)) {
            battle_action_decrement_player_item_quantity(unit, item & 0xff, 1);
            unit->action_taken = 1;
        }
        return BATTLE_ATTACK_PREPARATION_IMMEDIATE;
    case ACTION_MENU_TYPE_WEAPON_INVENTORY:
        ability = ABILITY_ID_THROW_FIRST;
        item = action.item_id;
        table = g_main_throw_ability_item_types - ABILITY_ID_THROW_FIRST;
        for (; ability < ABILITY_ID_JUMP_FIRST; ability++) {
            /* The index+base integer spelling of the biased table is byte-required. */
            if (*(u8*)(ability + (s32)table) == g_main_item_primary_data[item].type) {
                break;
            }
        }
        if (ability == ABILITY_ID_JUMP_FIRST) {
            return BATTLE_ATTACK_PREPARATION_INVALID;
        }
        main_util_copy_action_data(&action, dest);
        dest->ability_id = ability;
        if (phase != 0 && (phase & 1)) {
            battle_action_decrement_player_item_quantity(unit, item, 1);
            unit->action_taken = 1;
        }
        return BATTLE_ATTACK_PREPARATION_IMMEDIATE;
    case ACTION_MENU_TYPE_ARITHMETICKS:
        ability = action.ability_id;
        type = action.calculator_type;
        multiplier = action.calculator_multiplier;
        if (type < ABILITY_ID_MATH_FIRST || type >= ABILITY_ID_REACTION_FIRST) {
            return BATTLE_ATTACK_PREPARATION_INVALID;
        }
        if (multiplier < ABILITY_ID_MATH_FIRST || multiplier >= ABILITY_ID_REACTION_FIRST) {
            return BATTLE_ATTACK_PREPARATION_INVALID;
        }
        if ((u16)ability >= ABILITY_ID_ITEM_FIRST) {
            return BATTLE_ATTACK_PREPARATION_INVALID;
        }
        main_util_copy_action_data(&action, dest);
        if (phase != 0 && (phase & 1)) {
            unit->action_taken = 1;
        }
        return BATTLE_ATTACK_PREPARATION_IMMEDIATE;
    case ACTION_MENU_TYPE_ELEMENTS:
        action.ability_id = battle_action_get_elemental_ability_id(unit);
        break;
    case ACTION_MENU_TYPE_KATANA_INVENTORY:
        item = action.item_id;
        if (item < 0x26 || item > 0x2f) {
            return BATTLE_ATTACK_PREPARATION_INVALID;
        }
        ability = item + 0x26;
        action.ability_id = ability;
        if (phase & 1) {
            battle_action_decrement_player_item_quantity(unit, item, 1);
        }
        break;
    case ACTION_MENU_TYPE_ATTACK:
        main_util_copy_action_data(&action, dest);
        if (phase != 0 && (phase & 1)) {
            unit->action_taken = 1;
        }
        dest->ability_id = 0;
        return BATTLE_ATTACK_PREPARATION_IMMEDIATE;
    case ACTION_MENU_TYPE_JUMP:
        main_util_copy_action_data(&action, dest);
        if (phase != 0) {
            ct = 50 / unit->attributes[UNIT_ATTRIBUTE_SPEED];
            if (ct == 0) {
                jump_ct = 1;
            } else {
                jump_ct = ct;
            }
            unit->charged_ability_ct = jump_ct;
            if (phase & 1) {
                unit->action_taken = 1;
                unit->movement_taken = 1;
                main_status_set_action_state(unit, MAIN_UNIT_ACTION_STATE_JUMPING);
            }
        }
        dest->ability_id = ABILITY_ID_JUMP_FIRST;
        return BATTLE_ATTACK_PREPARATION_JUMP;
    case ACTION_MENU_TYPE_CHARGE:
        ability = action.ability_id;
        if (ability < ABILITY_ID_CHARGE_FIRST || ability >= ABILITY_ID_MATH_FIRST) {
            return BATTLE_ATTACK_PREPARATION_INVALID;
        }
        main_util_copy_action_data(&action, dest);
        ct = g_main_charge_ability_data[ability - ABILITY_ID_CHARGE_FIRST].ct;
        do {
            skip = 1;
        } while (0);
        if (phase & 1) {
            unit->movement_taken = 1;
        }
        persevere = 0;
        break;
    case ACTION_MENU_TYPE_DEFEND:
        return BATTLE_ATTACK_PREPARATION_CHARGE;
    case ACTION_MENU_TYPE_CHANGE_EQUIPMENT:
        return BATTLE_ATTACK_PREPARATION_CHANGE_EQUIPMENT;
    case ACTION_MENU_TYPE_DEFAULT:
    case ACTION_MENU_TYPE_MONSTER:
        break;
    default:
        return BATTLE_ATTACK_PREPARATION_INVALID;
    }
    if (!skip) {
        ability = action.ability_id;
        if ((u16)ability >= ABILITY_ID_ITEM_FIRST) {
            return BATTLE_ATTACK_PREPARATION_INVALID;
        }
        main_util_copy_action_data(&action, dest);
        entry = &g_main_ability_range_data[ability];
        ct = entry->ct & 0x7f;
        persevere = entry->flags_3 & ABILITY_SECONDARY_FLAG_3_PERSEVERE;
        support = unit->support_abilities[3];
        if (!persevere) {
            if (support & BATTLE_SUPPORT_SET_4_SHORT_CHARGE) {
                ct = (ct + 1) >> 1;
            }
            if (support & BATTLE_SUPPORT_SET_4_NON_CHARGE) {
                ct = 0;
            }
        }
    }
    if (phase != 0) {
        unit->charged_ability_ct = ct;
        if (phase & 1) {
            if (g_battle_action_context != BATTLE_ACTION_CONTEXT_PRIMARY) {
                return ct == 0;
            }
            unit->action_taken = 1;
            if (ct != 0) {
                if (persevere) {
                    main_status_set_action_state(unit, MAIN_UNIT_ACTION_STATE_PERFORMING);
                } else {
                    main_status_set_action_state(unit, MAIN_UNIT_ACTION_STATE_CHARGING);
                }
            }
        }
    }
    return ct == 0;
}
