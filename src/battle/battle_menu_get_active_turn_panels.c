#include "fft/battle.h"
#include "psx/types.h"

/* Copy the active-turn banner, active-unit record, billboard and selected-tile records into four
 * caller buffers. The help menus (helpmenu_run_battle_help_menu, helpmenu_menu_run_require_help)
 * take their copies this way. */
void battle_menu_get_active_turn_panels(void* banner, void* unit, void* billboard, void* cursor_tile) {
    battle_copy_bytes(banner, &g_battle_menu_active_turn_banner, sizeof(g_battle_menu_active_turn_banner));
    battle_copy_bytes(unit, &g_battle_active_turn_unit, sizeof(g_battle_active_turn_unit));
    battle_copy_bytes(billboard, g_battle_menu_billboard_data, sizeof(g_battle_menu_billboard_data));
    battle_copy_bytes(cursor_tile, &g_battle_map_selected_tile_data, sizeof(g_battle_map_selected_tile_data));
}
