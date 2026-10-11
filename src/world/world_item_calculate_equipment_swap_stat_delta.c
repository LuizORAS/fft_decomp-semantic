#include "fft/world.h"
#include "psx/types.h"

/* Accumulates the stat deltas of all equipment slot changes into total. */
void world_item_calculate_equipment_swap_stat_delta(
    menu_item_stat_detail_t* detail_total, menu_unit_status_record_t* total, u16* items_before, u16* items_after) {
    menu_unit_status_record_t delta;
    menu_item_stat_detail_t detail;
    menu_item_stat_detail_t unused;
    s32 slot;

    world_formation_clear_stat_preview(total, detail_total);
    for (slot = 0; slot < WORLD_EQUIPMENT_SLOT_COUNT; slot++) {
        world_item_calculate_swap_stat_delta(
            &detail, &delta, items_before[slot] & WORLD_ITEM_ID_MASK, items_after[slot] & WORLD_ITEM_ID_MASK, slot);
        world_item_combine_stat_details(detail_total, detail_total, &detail, -1);
        total->gauges[MENU_UNIT_STATUS_GAUGE_HP].bonus += delta.gauges[MENU_UNIT_STATUS_GAUGE_HP].bonus;
        total->gauges[MENU_UNIT_STATUS_GAUGE_MP].bonus += delta.gauges[MENU_UNIT_STATUS_GAUGE_MP].bonus;
    }
}
