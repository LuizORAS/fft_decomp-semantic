#include "fft/world.h"

/*
 * Combine the stat-detail fields used by equipment comparisons.
 *
 * Multiplier 1 subtracts before from after; -1 adds them. Callers can alias
 * output with before when accumulating equipment changes. Other fields remain
 * unchanged.
 */
void world_item_combine_stat_details(menu_status_panel_slot_storage_t* output, menu_status_panel_slot_storage_t* before,
    menu_status_panel_slot_storage_t* after, s32 multiplier) {
    output->state.move = after->state.move - multiplier * before->state.move;
    output->state.speed = after->state.speed - multiplier * before->state.speed;
    output->state.jump = after->state.jump - multiplier * before->state.jump;
    output->state.right_weapon_power = after->state.right_weapon_power - multiplier * before->state.right_weapon_power;
    output->state.left_weapon_power = after->state.left_weapon_power - multiplier * before->state.left_weapon_power;
    output->state.right_weapon_evade = after->state.right_weapon_evade - multiplier * before->state.right_weapon_evade;
    output->state.left_weapon_evade = after->state.left_weapon_evade - multiplier * before->state.left_weapon_evade;
    output->state.physical_attack = after->state.physical_attack - multiplier * before->state.physical_attack;
    output->state.physical_class_evade
        = after->state.physical_class_evade - multiplier * before->state.physical_class_evade;
    output->state.physical_shield_evade
        = after->state.physical_shield_evade - multiplier * before->state.physical_shield_evade;
    output->state.physical_accessory_evade
        = after->state.physical_accessory_evade - multiplier * before->state.physical_accessory_evade;
    output->state.magical_attack = after->state.magical_attack - multiplier * before->state.magical_attack;
    output->state.magical_class_evade
        = after->state.magical_class_evade - multiplier * before->state.magical_class_evade;
    output->state.magical_shield_evade
        = after->state.magical_shield_evade - multiplier * before->state.magical_shield_evade;
    output->state.magical_accessory_evade
        = after->state.magical_accessory_evade - multiplier * before->state.magical_accessory_evade;
}
