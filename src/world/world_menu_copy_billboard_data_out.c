#include "fft/memory.h"
#include "fft/world.h"

/* Fill four caller buffers from the default tables. */
void world_menu_copy_billboard_data_out(menu_unit_status_record_t* status, menu_active_unit_t* identity,
    menu_status_panel_slot_storage_t* detail, map_selected_tile_t* selected_tile) {
    mem_copy_bytes_nonempty(
        status, &g_world_unit_selected_status_billboard, sizeof(g_world_unit_selected_status_billboard));
    mem_copy_bytes_nonempty(identity, &g_world_active_unit_data, sizeof(g_world_active_unit_data));
    mem_copy_bytes_nonempty(detail, &g_world_unit_editor_fields, sizeof(g_world_unit_editor_fields));
    mem_copy_bytes_nonempty(selected_tile, &g_world_map_selected_tile_data, sizeof(g_world_map_selected_tile_data));
}
