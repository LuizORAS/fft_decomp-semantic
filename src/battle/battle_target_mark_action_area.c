#include "fft/battle_action.h"
#include "fft/battle_target.h"
#include "psx/types.h"

/* Mark the tiles an action hits, by the menu type of its skillset: an ability's area
 * (battle_target_mark_ability_area, with the ability Elemental or Draw Out picks), the units that pass
 * an Arithmeticks test (battle_target_run_calculator), and only the target tile for items, weapons
 * and Jump (battle_target_mark_only_target_tile).
 *
 * Returns -1 for an absent actor, an out-of-range unit target or an invalid ability, 2 for menu
 * types with no targeting, otherwise 1 when the routine returns 0, -1 when it returns -1, and 0 for
 * any other result. The target reads the actor's stats pointer in $a0 as
 * battle_action_get_elemental_ability_id's argument, so it is declared with one here. */
s32 battle_target_mark_action_area(const action_command_t* source) {
    action_command_t action;
    battle_stats_t* unit;
    u16 ability_id;
    u8 item;
    s32 mode;
    s32 result;

    main_util_copy_action_data(source, &action);
    mode = 1;
    if (action.actor_id >= BATTLE_UNIT_SLOT_COUNT) {
        return -1;
    }
    unit = &g_battle_unit_stats[action.actor_id];
    if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE) {
        return -1;
    }
    if (action.targeting_type == BATTLE_TARGETING_TYPE_UNIT && action.target_id >= BATTLE_UNIT_SLOT_COUNT) {
        return -1;
    }
    switch (g_main_action_menu_types_by_skillset[action.skillset_id]) {
    case ACTION_MENU_TYPE_DEFAULT:
    case ACTION_MENU_TYPE_MONSTER:
        ability_id = action.ability_id;
        break;
    case ACTION_MENU_TYPE_ITEM_INVENTORY:
    case ACTION_MENU_TYPE_WEAPON_INVENTORY:
    case ACTION_MENU_TYPE_ATTACK:
    case ACTION_MENU_TYPE_JUMP:
    case ACTION_MENU_TYPE_CHARGE:
        mode = 2;
        break;
    case ACTION_MENU_TYPE_ARITHMETICKS:
        result = battle_target_run_calculator(&action);
        mode = 0;
        break;
    case ACTION_MENU_TYPE_ELEMENTS:
        ability_id = battle_action_get_elemental_ability_id(unit);
        break;
    case ACTION_MENU_TYPE_KATANA_INVENTORY:
        item = action.item_or_equipment;
        if (item < ITEM_ID_ASURA_KNIFE || item > ITEM_ID_CHIRIJIRADEN) {
            return -1;
        }
        ability_id = (u8)action.item_or_equipment + (ABILITY_ID_DRAW_OUT_ASURA - ITEM_ID_ASURA_KNIFE);
        break;
    case ACTION_MENU_TYPE_BLANK_05:
    case ACTION_MENU_TYPE_UNKNOWN_0F:
        return -1;
    default:
        return 2;
    }
    if (mode == 1) {
        if (ability_id >= ABILITY_ID_ITEM_FIRST) {
            return -1;
        }
        action.ability_id = ability_id;
        result = battle_target_mark_ability_area(&action);
    }
    if (mode == 2) {
        result = battle_target_mark_only_target_tile(&action);
    }
    if (result == 0) {
        return 1;
    }
    return -(result == -1);
}
