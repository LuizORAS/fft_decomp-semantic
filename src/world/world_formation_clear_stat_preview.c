#include "fft/world.h"

/* Clear the fields used by equipment and ability stat previews; world_item_populate_stat_preview,
 * world_item_calculate_equipment_swap_stat_delta, world_ability_find_stat_changes_due_to_equipped and
 * world_ability_get_move_and_jump_increase_values start with it. */
void world_formation_clear_stat_preview(world_item_stat_summary_t* summary, world_item_stat_detail_t* detail) {
    summary->hp_bonus = 0;
    summary->mp_bonus = 0;
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
