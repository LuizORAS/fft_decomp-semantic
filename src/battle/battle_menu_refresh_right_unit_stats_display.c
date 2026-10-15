#include "fft/battle.h"
#include "psx/types.h"

/* Thread entry of the right-side unit stats panel: load the selected unit and run
 * the panel with the status billboard. The display mode is 2, or 0x0c00 when the
 * billboard's list_index is negative. */
void battle_menu_refresh_right_unit_stats_display(void) {
    s16* display_mode;

    battle_menu_load_selected_unit();
    display_mode = &g_battle_menu_right_unit_display_mode;
    *display_mode = 2;
    if (g_battle_menu_status_billboard.list_index < 0) {
        *display_mode = 0x0c00;
    }
    battle_menu_display_hovered_unit_stats(&g_battle_menu_right_unit_stats_display, UNIT_STATS_PANEL_RIGHT_UNIT,
        (menu_number_entry_t*)((u8*)display_mode - 0x50), &g_battle_menu_status_billboard);
}
