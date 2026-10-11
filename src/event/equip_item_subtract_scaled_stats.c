#include "fft/event_equip.h"
#include "psx/types.h"

/* Store `base - scale * scaled` for each 16-bit equipment stat of an item detail record. */
void equip_item_subtract_scaled_stats(menu_status_panel_slot_storage_t* out, menu_status_panel_slot_storage_t* scaled,
    menu_status_panel_slot_storage_t* base, s32 scale) {
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
