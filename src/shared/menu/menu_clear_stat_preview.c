#include "fft/menu.h"

/* Clear the fields used by equipment and ability stat previews; world_item_populate_stat_preview,
 * world_item_calculate_equipment_swap_stat_delta, world_ability_find_stat_changes_due_to_equipped and
 * world_ability_get_move_and_jump_increase_values start with it. */
void menu_clear_stat_preview(battle_menu_unit_status_record_t* summary, world_item_stat_detail_t* detail) {
    summary->gauges[BATTLE_MENU_UNIT_STATUS_GAUGE_HP].bonus = 0;
    summary->gauges[BATTLE_MENU_UNIT_STATUS_GAUGE_MP].bonus = 0;
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
