#include "fft/world.h"
#include "psx/types.h"

void world_menu_copy_billboard_data_in(const void* source_a, const void* source_b, const void* source_c) {
    world_script_copy_bytes(&g_world_unit_selected_status_billboard, source_a, 0x22);
    world_script_copy_bytes(g_world_active_unit_data, source_b, 0x0E);
    world_script_copy_bytes(g_world_unit_editor_fields, source_c, 0x40);
}
