#include "fft/battle_action.h"
#include "fft/battle_map.h"
#include "fft/battle_menu.h"
#include "fft/memory.h"
#include "fft/menu.h"

/* Copy the selected panel and tile records into four caller-owned buffers. */
void menu_copy_billboard_data_out(menu_unit_status_record_t* banner, menu_active_unit_t* unit,
    menu_status_panel_slot_storage_t* billboard, map_selected_tile_t* cursor_tile) {
    mem_copy_bytes_nonempty(banner, &g_menu_selected_unit_status, sizeof(g_menu_selected_unit_status));
    mem_copy_bytes_nonempty(unit, &g_menu_selected_unit, sizeof(g_menu_selected_unit));
    mem_copy_bytes_nonempty(billboard, &g_menu_selected_unit_fields, sizeof(g_menu_selected_unit_fields));
    mem_copy_bytes_nonempty(cursor_tile, &g_map_selected_tile_data, sizeof(g_map_selected_tile_data));
}
