#include "fft/battle.h"
#include "psx/types.h"

/* Pick the weapon the current strike uses. */
void battle_action_store_used_weapon(battle_action_command_t* action) {
    u8 weapon;
    s16 ability_id;

    if (g_battle_action_current_ability.strike_counter == 0) {
        weapon = g_battle_action_current_ability.primary_weapon_id;
    } else {
        weapon = g_battle_action_current_ability.secondary_weapon_id;
    }
    switch (g_main_action_menu_types_by_skillset[action->skillset_id]) {
    case ACTION_MENU_TYPE_DEFAULT:
    case ACTION_MENU_TYPE_ARITHMETICKS:
    case ACTION_MENU_TYPE_ELEMENTS:
    case ACTION_MENU_TYPE_MONSTER:
        ability_id = action->ability_id;
        if (ability_id < ABILITY_ID_ITEM_FIRST) {
            if ((g_main_ability_range_data[ability_id].flags_1
                    & (ABILITY_SECONDARY_FLAG_1_WEAPON_RANGE | ABILITY_SECONDARY_FLAG_1_WEAPON_STRIKE))
                == 0) {
                weapon = ITEM_ID_NOTHING;
            }
        }
        break;
    case ACTION_MENU_TYPE_ITEM_INVENTORY:
    case ACTION_MENU_TYPE_KATANA_INVENTORY:
        /* Retail reads only the low byte of the item/equipment word. */
        weapon = (u8)action->item_or_equipment;
        break;
    case ACTION_MENU_TYPE_JUMP:
    case ACTION_MENU_TYPE_UNKNOWN_0F:
        weapon = ITEM_ID_NOTHING;
        break;
    }
    if (weapon == ITEM_ID_NONE) {
        weapon = ITEM_ID_NOTHING;
    }
    g_battle_action_current_ability.weapon_id = weapon;
}
