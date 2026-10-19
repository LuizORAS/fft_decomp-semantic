#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_menu.h"
#include "fft/memory.h"

/* Copy an active-turn banner, an active-unit record and a billboard into the records the turn panels
 * show (g_menu_selected_unit_status, g_menu_selected_unit, g_menu_billboard_data).
 * REQUIRE and DEBUGCHR show the unit being edited this way. */
void battle_menu_set_active_turn_panels(const battle_menu_unit_status_record_t* banner,
    const battle_menu_active_unit_t* unit, const world_item_stat_detail_t* billboard) {
    mem_copy_bytes_nonempty(&g_menu_selected_unit_status, banner, sizeof(g_menu_selected_unit_status));
    mem_copy_bytes_nonempty(&g_menu_selected_unit, unit, sizeof(g_menu_selected_unit));
    mem_copy_bytes_nonempty(&g_menu_billboard_data, billboard, sizeof(g_menu_billboard_data));
}
