#include "fft/battle_menu.h"
#include "fft/menu.h"

/* Thread entry of the right-side unit stats panel: load the selected unit and run
 * the panel with the status billboard. The display mode is 2, or 0x0c00 (dashes) when the
 * billboard's list_index is negative. WORLD compiles this text too. */
void menu_refresh_right_unit_stats_display(void) {
    battle_menu_load_selected_unit();
    g_battle_menu_right_unit_stats_setup[6].format = 2;
    if (g_menu_comparison_unit_status.list_index < 0) {
        g_battle_menu_right_unit_stats_setup[6].format = 0x0c00;
    }
    battle_menu_display_hovered_unit_stats(&g_menu_right_unit_stats_rect, UNIT_STATS_PANEL_RIGHT_UNIT,
        g_battle_menu_right_unit_stats_setup, &g_menu_comparison_unit_status);
}
