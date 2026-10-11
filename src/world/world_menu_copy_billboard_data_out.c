#include "fft/world.h"

/* Fill four caller buffers from the default tables. */
void world_menu_copy_billboard_data_out(menu_unit_status_record_t* status, menu_active_unit_t* identity,
    menu_status_panel_slot_storage_t* detail, map_selected_tile_t* selected_tile) {
    world_script_copy_bytes(
        status, &g_world_unit_selected_status_billboard, sizeof(g_world_unit_selected_status_billboard));
    world_script_copy_bytes(identity, &g_world_active_unit_data, sizeof(g_world_active_unit_data));
    world_script_copy_bytes(detail, &g_world_unit_editor_fields, sizeof(g_world_unit_editor_fields));
    world_script_copy_bytes(selected_tile, &g_world_map_selected_tile_data, sizeof(g_world_map_selected_tile_data));
}
