#include "fft/world.h"

/* Fill four caller buffers from the default tables. */
void world_menu_get_active_turn_panels(void* dest0, void* dest1, void* dest2, void* dest3) {
    world_copy_bytes(dest0, &g_world_menu_active_turn_banner, sizeof(g_world_menu_active_turn_banner));
    world_copy_bytes(dest1, &g_world_active_turn_unit, sizeof(g_world_active_turn_unit));
    world_copy_bytes(dest2, &g_world_menu_billboard_data, sizeof(g_world_menu_billboard_data));
    world_copy_bytes(dest3, &g_world_map_selected_tile_data, sizeof(g_world_map_selected_tile_data));
}
