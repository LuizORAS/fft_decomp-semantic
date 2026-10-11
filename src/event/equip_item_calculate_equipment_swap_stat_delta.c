#include "fft/event_equip.h"
#include "psx/types.h"

/* Sum the stat changes of swapping each of the five equipment slots from `from_items` to `to_items`. */
void equip_item_calculate_equipment_swap_stat_delta(
    world_item_stat_detail_t* detail_total, world_item_stat_summary_t* summary_total, u16* from_items, u16* to_items) {
    world_item_stat_summary_t summary_delta;
    world_item_stat_detail_t detail_delta;
    world_item_stat_detail_t unused; /* unreferenced in the target; needed for the frame size */
    s32 i;

    equip_item_clear_stat_sums(summary_total, detail_total);

    for (i = 0; i < 5; i++) {
        equip_item_calculate_swap_stat_delta(
            &detail_delta, &summary_delta, from_items[i] & EQUIP_ITEM_ID_MASK, to_items[i] & EQUIP_ITEM_ID_MASK, i);
        equip_item_subtract_scaled_stats(detail_total, detail_total, &detail_delta, -1);
        summary_total->hp_bonus += summary_delta.hp_bonus;
        summary_total->mp_bonus += summary_delta.mp_bonus;
    }
}
