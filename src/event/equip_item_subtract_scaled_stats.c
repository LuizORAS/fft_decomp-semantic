#include "fft/event_equip.h"
#include "psx/types.h"

/* Store `base - scale * scaled` for each equipment-stat halfword of an item detail record. */
void equip_item_subtract_scaled_stats(
    menu_item_stat_detail_t* out, menu_item_stat_detail_t* scaled, menu_item_stat_detail_t* base, s32 scale) {
    out->move_bonus = base->move_bonus - (scale * scaled->move_bonus);
    out->speed_bonus = base->speed_bonus - (scale * scaled->speed_bonus);
    out->jump_bonus = base->jump_bonus - (scale * scaled->jump_bonus);
    out->right_weapon_power = base->right_weapon_power - (scale * scaled->right_weapon_power);
    out->left_weapon_power = base->left_weapon_power - (scale * scaled->left_weapon_power);
    out->right_weapon_evade = base->right_weapon_evade - (scale * scaled->right_weapon_evade);
    out->left_weapon_evade = base->left_weapon_evade - (scale * scaled->left_weapon_evade);
    out->physical_attack_bonus = base->physical_attack_bonus - (scale * scaled->physical_attack_bonus);
    out->physical_class_evade = base->physical_class_evade - (scale * scaled->physical_class_evade);
    out->physical_shield_evade = base->physical_shield_evade - (scale * scaled->physical_shield_evade);
    out->physical_accessory_evade = base->physical_accessory_evade - (scale * scaled->physical_accessory_evade);
    out->magical_attack_bonus = base->magical_attack_bonus - (scale * scaled->magical_attack_bonus);
    out->magical_class_evade = base->magical_class_evade - (scale * scaled->magical_class_evade);
    out->magical_shield_evade = base->magical_shield_evade - (scale * scaled->magical_shield_evade);
    out->magical_accessory_evade = base->magical_accessory_evade - (scale * scaled->magical_accessory_evade);
}
