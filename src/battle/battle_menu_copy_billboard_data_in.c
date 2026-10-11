#include "fft/battle.h"
#include "psx/types.h"

/* Load the active-turn banner, the active-turn unit record and the billboard data
 * from the caller's copies: the inverse of battle_menu_copy_billboard_data_out,
 * without the cursor tile record. REQUIRE.OUT publishes its own records this way. */
void battle_menu_copy_billboard_data_in(const menu_unit_status_record_t* banner, const menu_active_unit_t* unit,
    const menu_status_panel_slot_storage_t* billboard) {
    battle_copy_bytes(&g_battle_menu_active_turn_banner, banner, sizeof(g_battle_menu_active_turn_banner));
    battle_copy_bytes(&g_battle_active_turn_unit, unit, sizeof(g_battle_active_turn_unit));
    battle_copy_bytes(&g_battle_menu_billboard_data, billboard, sizeof(g_battle_menu_billboard_data));
}
