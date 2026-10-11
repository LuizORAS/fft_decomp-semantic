#include "fft/event_equip.h"
#include "psx/types.h"

/* Zero the HP/MP bonus halfwords of an item stat summary and the stat block of its detail record. */
void equip_item_clear_stat_sums(menu_unit_status_record_t* summary, menu_item_stat_detail_t* detail) {
    summary->gauges[MENU_UNIT_STATUS_GAUGE_HP].bonus = 0;
    summary->gauges[MENU_UNIT_STATUS_GAUGE_MP].bonus = 0;
    detail->move_bonus = 0;
    detail->speed_bonus = 0;
    detail->jump_bonus = 0;
    detail->right_weapon_power = 0;
    detail->left_weapon_power = 0;
    detail->left_weapon_evade = 0;
    detail->right_weapon_evade = 0;
    detail->physical_attack_bonus = 0;
    detail->physical_class_evade = 0;
    detail->physical_shield_evade = 0;
    detail->physical_accessory_evade = 0;
    detail->magical_attack_bonus = 0;
    detail->magical_class_evade = 0;
    detail->magical_shield_evade = 0;
    detail->magical_accessory_evade = 0;
}
