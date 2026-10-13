#include "fft/event_equip.h"
#include "psx/types.h"

/* Row callback 6 of the slot item browser (installed by equip_menu_run_slot_item_browser): the HP bonus
 * of the previewed equipment (g_equip_selected_unit_stat_summary.hp_bonus). Callback 4 shows the same value
 * with dashes for 0. */
s32 equip_menu_get_preview_hp_bonus(s32 unused_row) {
    return g_equip_selected_unit_stat_summary.hp_bonus;
}
