#include "fft/event_equip.h"
#include "psx/types.h"

/* Sum the stat bonuses of the five equipped items listed in `equipment`. */
void equip_item_sum_equipment_stat_details(world_item_stat_detail_t* total, const u16* equipment) {
    battle_menu_unit_status_record_t summary;
    world_item_stat_detail_t detail;
    world_item_stat_detail_t unused; /* unreferenced in the target; needed for the frame size */
    s32 i;

    total->state.right_weapon_power = 0;
    total->state.left_weapon_power = 0;
    total->state.left_weapon_evade = 0;
    total->state.right_weapon_evade = 0;
    total->state.physical_shield_evade = 0;
    total->state.physical_accessory_evade = 0;
    total->state.magical_shield_evade = 0;
    total->state.magical_accessory_evade = 0;
    for (i = 0; i < 5; i++) {
        equip_item_collect_stat_deltas(equipment[i] & EQUIP_ITEM_ID_MASK, &summary, &detail, i);
        total->state.right_weapon_power += detail.state.right_weapon_power;
        total->state.left_weapon_power += detail.state.left_weapon_power;
        total->state.right_weapon_evade += detail.state.right_weapon_evade;
        total->state.left_weapon_evade += detail.state.left_weapon_evade;
        total->state.physical_shield_evade += detail.state.physical_shield_evade;
        total->state.physical_accessory_evade += detail.state.physical_accessory_evade;
        total->state.magical_shield_evade += detail.state.magical_shield_evade;
        total->state.magical_accessory_evade += detail.state.magical_accessory_evade;
    }
}
