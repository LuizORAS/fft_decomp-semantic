#include "fft/battle.h"
#include "psx/types.h"

/* Load the unit's usable entries of one skillset for its action menu type into scratch lists,
 * through the type's loader: the ability list (default), the Item abilities, the throwable items
 * (Throw), Math Skill's attributes, then multiples, then the Calculator abilities, the Elemental
 * ability of the unit's tile, the Draw Out katanas or the Charge abilities. A Frog gets 0 for every
 * type but the default, blank, monster and attack menus. Returns the loader's result, 0 when nothing
 * is usable, or for types without a loader whether the type is below 15;
 * battle_menu_build_unit_action_menus marks a skillset empty on 0. */
s32 battle_menu_load_skillset(s32 unit_id, u8 skillset, u8 menu) {
    u8 items[0x58];
    s16 abilities[0x44];
    u8 mp_costs[0x18];
    u8 ability_ct[0x18];
    u8 ability_flags[0x18];
    u8 turns[0x18];
    s32 result;

    if ((g_battle_unit_stats[unit_id].status_sets.current[2] & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FROG))
        && menu != ACTION_MENU_TYPE_DEFAULT && menu != ACTION_MENU_TYPE_MONSTER && menu != ACTION_MENU_TYPE_ATTACK
        && menu != ACTION_MENU_TYPE_BLANK_05) {
        return 0;
    }
    switch (menu) {
    case ACTION_MENU_TYPE_DEFAULT:
        return battle_menu_get_unit_skillset_ability_data(
            unit_id, skillset, abilities, mp_costs, ability_ct, 0, ability_flags, turns);
    case ACTION_MENU_TYPE_ITEM_INVENTORY:
        return battle_menu_load_item_abilities(unit_id, skillset, items);
    case ACTION_MENU_TYPE_WEAPON_INVENTORY:
        return battle_menu_collect_throwable_items(unit_id, skillset, items);
    case ACTION_MENU_TYPE_ARITHMETICKS:
        result = battle_menu_load_math_skill_attributes(unit_id, skillset, abilities);
        if (result != 0) {
            result = battle_menu_load_math_skill_multiples(unit_id, skillset, abilities);
            if (result != 0) {
                result = battle_menu_collect_calculator_abilities(unit_id, skillset, abilities);
            }
        }
        return result;
    case ACTION_MENU_TYPE_ELEMENTS:
        return (u16)battle_menu_load_elemental_abilities(unit_id, skillset);
    case ACTION_MENU_TYPE_KATANA_INVENTORY:
        return battle_menu_collect_draw_out_katanas(unit_id, skillset, items);
    case ACTION_MENU_TYPE_CHARGE:
        return battle_menu_call_load_charge_skillset(unit_id, skillset, abilities);
    default:
        return menu < 15;
    }
}
