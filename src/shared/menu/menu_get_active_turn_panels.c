#include "fft/battle_action.h"
#include "fft/battle_map.h"
#include "fft/battle_menu.h"
#include "fft/memory.h"
#include "fft/menu.h"

/* Copy the active-turn banner, active-unit record, billboard and selected-tile records into four
 * caller buffers. The help menus (helpmenu_menu_run_battle_help, helpmenu_menu_run_require_help)
 * take their copies this way. WORLD compiles this text too. */
void menu_get_active_turn_panels(battle_menu_unit_status_record_t* banner, battle_menu_active_unit_t* unit,
    world_item_stat_detail_t* billboard, map_selected_tile_t* cursor_tile) {
    mem_copy_bytes_nonempty(banner, &g_menu_selected_unit_status, sizeof(g_menu_selected_unit_status));
    mem_copy_bytes_nonempty(unit, &g_menu_selected_unit, sizeof(g_menu_selected_unit));
    mem_copy_bytes_nonempty(billboard, &g_menu_billboard_data, sizeof(g_menu_billboard_data));
    mem_copy_bytes_nonempty(cursor_tile, &g_map_selected_tile_data, sizeof(g_map_selected_tile_data));
}
