#include "fft/event_equip.h"

/* Zero the 16-bit HP/MP bonuses of an item stat summary and the stat block of its detail record. */
void equip_item_clear_stat_sums(menu_unit_status_record_t* summary, menu_status_panel_slot_storage_t* detail) {
    summary->gauges[MENU_UNIT_STATUS_GAUGE_HP].bonus = 0;
    summary->gauges[MENU_UNIT_STATUS_GAUGE_MP].bonus = 0;
    detail->state.move = 0;
    detail->state.speed = 0;
    detail->state.jump = 0;
    detail->state.right_weapon_power = 0;
    detail->state.left_weapon_power = 0;
    detail->state.left_weapon_evade = 0;
    detail->state.right_weapon_evade = 0;
    detail->state.physical_attack = 0;
    detail->state.physical_class_evade = 0;
    detail->state.physical_shield_evade = 0;
    detail->state.physical_accessory_evade = 0;
    detail->state.magical_attack = 0;
    detail->state.magical_class_evade = 0;
    detail->state.magical_shield_evade = 0;
    detail->state.magical_accessory_evade = 0;
}
