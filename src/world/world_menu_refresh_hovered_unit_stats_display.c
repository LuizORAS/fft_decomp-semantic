#include "fft/world.h"
#include "psx/types.h"

void world_menu_refresh_hovered_unit_stats_display(void) {
    world_menu_load_selected_unit();
    g_world_menu_hovered_unit_stats_setup[6].format = 2;
    if (g_world_unit_selected_status_billboard.list_index < 0)
        g_world_menu_hovered_unit_stats_setup[6].format = 0xc00;
    world_menu_display_hovered_unit_stats(&g_world_menu_hovered_unit_stats_display, UNIT_STATS_PANEL_HOVERED_UNIT,
        g_world_menu_hovered_unit_stats_setup, &g_world_unit_selected_status_billboard);
}
