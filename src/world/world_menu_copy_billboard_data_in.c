#include "fft/world.h"
#include "psx/types.h"

void world_menu_copy_billboard_data_in(const void* source_a, const void* source_b, const void* source_c) {
    world_script_copy_bytes(
        &g_world_unit_selected_status_billboard, source_a, sizeof(g_world_unit_selected_status_billboard));
    world_script_copy_bytes(&g_world_active_unit_data, source_b, sizeof(g_world_active_unit_data));
    world_script_copy_bytes(&g_world_unit_editor_fields, source_c, sizeof(g_world_unit_editor_fields));
}
