#include "fft/battle_menu.h"
#include "fft/menu.h"

/* Reload the unit and redraw its status panel; a negative list index draws dashes. */
void menu_refresh_right_unit_stats_display(void) {
    battle_menu_load_selected_unit();
    g_battle_menu_right_unit_stats_setup[6].format = 2;
    if (g_battle_menu_status_billboard.list_index < 0) {
        g_battle_menu_right_unit_stats_setup[6].format = 0x0c00;
    }
    battle_menu_display_hovered_unit_stats(&g_battle_menu_right_unit_stats_display, UNIT_STATS_PANEL_RIGHT_UNIT,
        g_battle_menu_right_unit_stats_setup, &g_battle_menu_status_billboard);
}
