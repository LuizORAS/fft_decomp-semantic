#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_map.h"
#include "fft/battle_move.h"
#include "psx/types.h"

/* g_battle_action_current_ability bound a second time. Spelling every field through one
 * symbol lets GCC share a base register between the two access groups below,
 * which changes this function's code. */
extern battle_current_ability_t g_battle_action_current_ability_view;

/* Pre-formula setup: loads the current ability, weapon and item data for one
 * attacker/target pair and runs the formula handler.
 *
 * Returns 1 when the target was already missed before the formula (reflect,
 * Blade Grasp, Arrow Guard) and 0 otherwise, including the reaction and
 * knockback early exits. id holds a weapon or item id, then a status-infliction
 * index. */
s32 battle_action_run_pre_formula_setup(const battle_action_command_t* source, u8 target_id) {
    battle_action_command_t action;
    weapon_data_t* weapon;
    ability_secondary_data_t* ability;
    item_secondary_data_t* item;
    ability_data_t* data;
    u8 menu_type;
    u8 skillset;
    u8 formula;
    u8 id;
    u16 ability_id;

    main_util_copy_action_data(source, &action);
    g_battle_action_current_ability.target_id = target_id;
    g_battle_action_target = &g_battle_unit_stats[target_id];
    g_battle_action_target_data = &g_battle_unit_stats[target_id].action;
    g_battle_action_current_ability.post_action_message = BATTLE_POST_ACTION_MESSAGE_NONE;
    g_battle_action_current_ability_view.attacker_id = action.actor_id;
    g_battle_action_attacker_data = &g_battle_action_attacker_action_data;
    g_battle_action_attacker = &g_battle_unit_stats[g_battle_action_current_ability_view.attacker_id];
    if (g_battle_action_context != BATTLE_ACTION_CONTEXT_PRIMARY && battle_action_perform_reaction_ability() != 0) {
        battle_action_store_used_weapon(&action);
        return 0;
    }
    if (g_battle_action_current_ability.knockback_flags & BATTLE_KNOCKBACK_PENDING) {
        battle_move_apply_knockback();
        return 0;
    }
    g_battle_action_current_ability_view.terrain
        = g_battle_map_tile_data[battle_map_calculate_location(g_battle_action_target)].surface.type;
    if (g_battle_action_current_ability.strike_counter == 0) {
        id = g_battle_action_current_ability.primary_weapon_id;
    } else {
        id = g_battle_action_current_ability.secondary_weapon_id;
    }
    if (action.skillset_id == SKILLSET_ID_DRAW_OUT) {
        /* Item selection reads only the low byte of the 16-bit item/equipment field. */
        id = action.item_or_equipment;
    }
    if (id >= 0x80) {
        id = ITEM_ID_NOTHING;
    }
    g_battle_action_current_ability_view.weapon_id = id;
    g_battle_action_current_ability_view.ability_id = action.ability_id;
    ability_id = action.ability_id;
    g_battle_action_current_ability.two_hands_active = 0;
    if (g_battle_action_current_ability_view.ability_id > ABILITY_ID_ITEM_FIRST) {
        ability_id = 0;
    }
    weapon = &g_main_item_weapon_data[id];
    main_util_copy_byte_data(&g_main_ability_range_data[ability_id], &g_battle_action_current_ability_view.range_data,
        sizeof(ability_secondary_data_t));
    main_util_copy_byte_data(weapon, &g_battle_action_current_ability_view.weapon_data, sizeof(weapon_data_t));
    g_battle_action_current_ability.proc_id = weapon->proc_id;
    g_battle_action_current_ability.charge_power = 0;
    g_battle_action_current_ability.formula = ABILITY_FORMULA_WEAPON_DAMAGE;
    g_battle_action_current_ability.attacker_faith = g_battle_action_attacker->faith;
    g_battle_action_current_ability.target_faith = g_battle_action_target->faith;
    battle_action_clear_data();
    skillset = action.skillset_id;
    g_battle_action_current_ability.skillset = skillset;
    menu_type = g_main_action_menu_types_by_skillset[skillset];
    if (g_battle_action_current_ability_view.weapon_proc_pending != 0) {
        action.ability_id = g_battle_action_current_ability_view.reaction_id;
        g_battle_action_current_ability_view.ability_id = action.ability_id;
        menu_type = ACTION_MENU_TYPE_DEFAULT;
    } else {
        g_battle_action_current_ability_view.reaction_id = 0;
    }
    data = &g_main_ability_data[action.ability_id];
    g_battle_action_target_data->miss_display_flags = data->ai_flags.flags_1 & BATTLE_ACTION_MISS_DISPLAY_TEAM_MASK;
    if (data->ai_flags.flags_4 & BATTLE_AI_ABILITY_FLAG_4_EVADE_WITH_MOTION) {
        g_battle_action_target_data->miss_display_flags += BATTLE_ACTION_MISS_DISPLAY_DODGE_ANIMATION;
    }
    if (menu_type == ACTION_MENU_TYPE_KATANA_INVENTORY) {
        id = action.item_or_equipment;
        action.ability_id = id + (ABILITY_ID_DRAW_OUT_ASURA - ITEM_ID_ASURA_KNIFE);
    }
    switch (menu_type) {
    case ACTION_MENU_TYPE_ELEMENTS:
        action.ability_id = battle_action_get_elemental_ability_id(g_battle_action_attacker);
        /* fall through */
    case ACTION_MENU_TYPE_DEFAULT:
    case ACTION_MENU_TYPE_ARITHMETICKS:
    case ACTION_MENU_TYPE_MONSTER:
    case ACTION_MENU_TYPE_KATANA_INVENTORY:
        formula = ABILITY_FORMULA_WEAPON_DAMAGE;
        if (action.ability_id <= ABILITY_ID_ITEM_FIRST) {
            if ((g_battle_action_attacker->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FROG)]
                    & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG))
                && g_battle_action_current_ability_view.weapon_proc_pending == 0) {
                g_battle_action_current_ability_view.weapon_id = ITEM_ID_NOTHING;
                main_util_copy_byte_data(
                    g_main_item_weapon_data, &g_battle_action_current_ability_view.weapon_data, sizeof(weapon_data_t));
            }
            ability = &g_main_ability_range_data[action.ability_id];
            main_util_copy_byte_data(
                ability, &g_battle_action_current_ability_view.range_data, sizeof(ability_secondary_data_t));
            g_battle_action_current_ability.proc_id = ability->status_infliction_id;
            formula = ability->formula;
            if (!(g_battle_action_current_ability.range_data.flags_1
                    & (ABILITY_SECONDARY_FLAG_1_WEAPON_RANGE | ABILITY_SECONDARY_FLAG_1_WEAPON_STRIKE))
                && menu_type != ACTION_MENU_TYPE_KATANA_INVENTORY) {
                g_battle_action_current_ability_view.weapon_id = ITEM_ID_NOTHING;
            }
        }
        break;
    case ACTION_MENU_TYPE_ITEM_INVENTORY:
        formula = ABILITY_FORMULA_WEAPON_DAMAGE;
        id = action.item_or_equipment;
        if (id >= ITEM_ID_CONSUMABLE_FIRST) {
            g_battle_action_current_ability_view.weapon_id = id;
            id += ITEM_ID_COUNT - ITEM_ID_CONSUMABLE_FIRST;
            item = &g_main_item_secondary_data[id];
            g_battle_action_current_ability.used_item_id = id;
            g_battle_action_current_ability.proc_id = item->status_infliction_id;
            main_util_clear_byte_data(
                &g_battle_action_current_ability_view.range_data, sizeof(ability_secondary_data_t));
            formula = item->formula;
        }
        break;
    case ACTION_MENU_TYPE_WEAPON_INVENTORY:
        formula = ABILITY_FORMULA_DAMAGE_SP_TIMES_WP;
        break;
    case ACTION_MENU_TYPE_JUMP:
        formula = ABILITY_FORMULA_DAMAGE_PA_TIMES_WP;
        break;
    case ACTION_MENU_TYPE_CHARGE:
        ability_id = action.ability_id;
        if (ability_id < ABILITY_ID_CHARGE_FIRST || ability_id >= ABILITY_ID_MATH_FIRST) {
            ability_id = ABILITY_ID_CHARGE_FIRST;
        }
        g_battle_action_current_ability.charge_power
            = (g_main_charge_ability_data - ABILITY_ID_CHARGE_FIRST)[ability_id].power;
        /* fall through */
    case ACTION_MENU_TYPE_ATTACK:
        formula = weapon->formula;
        if ((g_battle_action_current_ability.weapon_data.flags & WEAPON_FLAG_TWO_HANDS_COMPATIBLE)
            && (g_battle_action_attacker->support_abilities[2] & BATTLE_SUPPORT_SET_3_TWO_HANDS)) {
            if (g_battle_action_attacker->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_WEAPON] == ITEM_ID_NONE
                && g_battle_action_attacker->equipment[UNIT_EQUIPMENT_SLOT_RIGHT_HAND_SHIELD] == ITEM_ID_NONE) {
                g_battle_action_current_ability.two_hands_active = 1;
            }
            if (g_battle_action_attacker->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_WEAPON] == ITEM_ID_NONE
                && g_battle_action_attacker->equipment[UNIT_EQUIPMENT_SLOT_LEFT_HAND_SHIELD] == ITEM_ID_NONE) {
                g_battle_action_current_ability.two_hands_active = 1;
            }
        }
        break;
    default:
        formula = ABILITY_FORMULA_WEAPON_DAMAGE;
        break;
    }
    g_battle_action_current_ability_view.weapon_proc_pending = 0;
    g_battle_action_current_ability.knockback_flags = 0;
    if (formula == 0 || formula >= ABILITY_FORMULA_ID_LIMIT) {
        formula = ABILITY_FORMULA_WEAPON_DAMAGE;
    }
    id = g_battle_action_current_ability_view.proc_id;
    if (formula == ABILITY_FORMULA_GUN_DAMAGE) {
        id = 0;
    }
    main_util_copy_byte_data(&g_main_status_infliction_data[id],
        &g_battle_action_current_ability_view.status_infliction, sizeof(status_infliction_data_t));
    battle_status_remove();
    g_battle_action_current_ability.formula = formula;
    battle_action_check_reflect_blade_grasp_and_arrow_guard();
    if (g_battle_action_target_data->hit == 0) {
        return 1;
    }
    g_battle_formula_handlers[formula - 1]();
    if (formula < 7) {
        battle_formula_apply_poach_and_train();
    }
    if (formula == ABILITY_FORMULA_DAMAGE_PA_TIMES_WP) {
        g_battle_action_current_ability_view.weapon_id = ITEM_ID_NOTHING;
    }
    battle_action_finalize_target_current_action();
    if (g_battle_action_target_data->special_effect != 0) {
        g_battle_action_target_data->attack_type |= BATTLE_ACTION_TYPE_PSEUDO_STATUS;
    }
    return 0;
}
