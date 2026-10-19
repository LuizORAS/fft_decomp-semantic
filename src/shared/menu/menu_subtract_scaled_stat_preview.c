#include "fft/menu.h"

/*
 * Combine the stat-detail fields used by equipment comparisons: out = base - scale * scaled,
 * so a scale of 1 subtracts scaled from base and -1 adds them. The inputs may alias the
 * output when callers accumulate equipment changes; other fields remain unchanged.
 */
void menu_subtract_scaled_stat_preview(world_item_stat_detail_t* out, const world_item_stat_detail_t* scaled,
    const world_item_stat_detail_t* base, s32 scale) {
    out->state.move = base->state.move - (scale * scaled->state.move);
    out->state.speed = base->state.speed - (scale * scaled->state.speed);
    out->state.jump = base->state.jump - (scale * scaled->state.jump);
    out->state.right_weapon_power = base->state.right_weapon_power - (scale * scaled->state.right_weapon_power);
    out->state.left_weapon_power = base->state.left_weapon_power - (scale * scaled->state.left_weapon_power);
    out->state.right_weapon_evade = base->state.right_weapon_evade - (scale * scaled->state.right_weapon_evade);
    out->state.left_weapon_evade = base->state.left_weapon_evade - (scale * scaled->state.left_weapon_evade);
    out->state.physical_attack = base->state.physical_attack - (scale * scaled->state.physical_attack);
    out->state.physical_class_evade = base->state.physical_class_evade - (scale * scaled->state.physical_class_evade);
    out->state.physical_shield_evade
        = base->state.physical_shield_evade - (scale * scaled->state.physical_shield_evade);
    out->state.physical_accessory_evade
        = base->state.physical_accessory_evade - (scale * scaled->state.physical_accessory_evade);
    out->state.magical_attack = base->state.magical_attack - (scale * scaled->state.magical_attack);
    out->state.magical_class_evade = base->state.magical_class_evade - (scale * scaled->state.magical_class_evade);
    out->state.magical_shield_evade = base->state.magical_shield_evade - (scale * scaled->state.magical_shield_evade);
    out->state.magical_accessory_evade
        = base->state.magical_accessory_evade - (scale * scaled->state.magical_accessory_evade);
}
