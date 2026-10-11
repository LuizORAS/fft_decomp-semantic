#include "fft/event_equip.h"

/* The value an item ranks by in the menus' descending sorts: a weapon's power, a shield's physical
 * evade, headgear's or body armor's HP bonus, and the item level of an accessory or any other item. */
s32 equip_item_get_ranking_value(s32 item_id) {
    s32 type;
    s32 idx;

    /* The target narrows item_id to s16 for this s32 parameter. */
    type = equip_item_get_menu_category(item_id);
    if (item_id != 0 && type == ITEM_MENU_CATEGORY_WEAPON) {
        return g_main_item_weapon_data[g_main_item_primary_data[item_id].secondary_data_id].power;
    }
    if (type == ITEM_MENU_CATEGORY_SHIELD) {
        return g_main_item_shield_data[g_main_item_primary_data[item_id].secondary_data_id].physical_evade;
    }
    if (type >= ITEM_MENU_CATEGORY_ACCESSORY) {
        return g_main_item_primary_data[item_id].required_level;
    }
    idx = item_id - ITEM_ID_HEADGEAR_FIRST;
    return g_main_item_helm_armor_secondary_data[idx].hp_bonus;
}
