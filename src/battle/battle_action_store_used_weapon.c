#include "fft/battle.h"
#include "psx/types.h"

/* Pick the weapon the current strike uses (g_current_ability.weapon_id): the first strike's hand,
 * then the second's. An ordinary, Math Skill, Geomancy or monster ability that neither uses the
 * weapon's range nor strikes with it uses none; Item and Draw Out use the command's item; Jump and
 * menu type 0x0f use none. An empty hand counts as ITEM_ID_NOTHING. */
void battle_action_store_used_weapon(const battle_ai_command_action_t* action) {
    u8 weapon;
    s16 ability_id;

    if (g_current_ability.strike_counter == 0) {
        weapon = g_current_ability.primary_weapon_id;
    } else {
        weapon = g_current_ability.secondary_weapon_id;
    }
    switch (g_main_action_menu_types_by_skillset[action->skillset]) {
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
        weapon = action->item_id;
        break;
    case ACTION_MENU_TYPE_JUMP:
    case ACTION_MENU_TYPE_UNKNOWN_0F:
        weapon = ITEM_ID_NOTHING;
        break;
    }
    if (weapon == ITEM_ID_NONE) {
        weapon = ITEM_ID_NOTHING;
    }
    g_current_ability.weapon_id = weapon;
}
