#include "fft/world.h"
#include "psx/types.h"

/* Reload the selected unit and redraw the right-hand (comparison) unit stats
 * panel. Number entry 6 (the list position) is drawn as dashes when the
 * billboard's list index is negative. */
void world_menu_refresh_right_unit_stats_display(void) {
    world_menu_load_selected_unit();
    g_world_menu_right_unit_stats_setup[6].format = 2;
    if (g_world_unit_comparison_status_billboard.list_index < 0)
        g_world_menu_right_unit_stats_setup[6].format = 0xc00;
    world_menu_display_hovered_unit_stats(&g_world_menu_right_unit_stats_display, UNIT_STATS_PANEL_RIGHT_UNIT,
        g_world_menu_right_unit_stats_setup, &g_world_unit_comparison_status_billboard);
}
