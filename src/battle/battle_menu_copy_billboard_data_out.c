#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_map.h"
#include "fft/battle_menu.h"

/* Snapshot the AT banner, active unit, billboard and cursor tile records into
   the four caller-owned buffers so the caller can restore them later. */
void battle_menu_copy_billboard_data_out(menu_unit_status_record_t* banner, menu_active_unit_t* unit,
    menu_status_panel_slot_storage_t* billboard, map_selected_tile_t* cursor_tile) {
    battle_copy_bytes(banner, &g_battle_menu_active_turn_banner, sizeof(g_battle_menu_active_turn_banner));
    battle_copy_bytes(unit, &g_battle_active_turn_unit, sizeof(g_battle_active_turn_unit));
    battle_copy_bytes(billboard, &g_battle_menu_billboard_data, sizeof(g_battle_menu_billboard_data));
    battle_copy_bytes(cursor_tile, &g_map_selected_tile_data, sizeof(g_map_selected_tile_data));
}
