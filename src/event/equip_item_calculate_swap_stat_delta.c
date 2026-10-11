#include "fft/event_equip.h"
#include "psx/types.h"

/* Compute the HP/MP and detailed stat changes for one equipment replacement. */
void equip_item_calculate_swap_stat_delta(
    world_item_stat_detail_t* output, battle_unit_status_record_t* delta, s16 item_before, s16 item_after, s32 slot) {
    battle_unit_status_record_t summary_before;
    battle_unit_status_record_t summary_after;
    world_item_stat_detail_t detail_before;
    world_item_stat_detail_t detail_after;

    equip_collect_item_stat_deltas(item_before, &summary_before, &detail_before, slot);
    equip_collect_item_stat_deltas(item_after, &summary_after, &detail_after, slot);
    delta->gauges[BATTLE_UNIT_STATUS_GAUGE_HP].bonus = summary_after.gauges[BATTLE_UNIT_STATUS_GAUGE_HP].bonus
        - summary_before.gauges[BATTLE_UNIT_STATUS_GAUGE_HP].bonus;
    delta->gauges[BATTLE_UNIT_STATUS_GAUGE_MP].bonus = summary_after.gauges[BATTLE_UNIT_STATUS_GAUGE_MP].bonus
        - summary_before.gauges[BATTLE_UNIT_STATUS_GAUGE_MP].bonus;
    equip_item_subtract_scaled_stats(output, &detail_before, &detail_after, 1);
}
