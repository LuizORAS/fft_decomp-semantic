#include "fft/battle.h"
#include "psx/types.h"

/* Thread entry of the hovered-unit stats panel: load the selected unit and run
 * the panel with the active-turn banner. The display mode is 2, or 0x0c00 when
 * the banner's list_index is negative. */
void battle_menu_refresh_hovered_unit_stats_display(void) {
    s16* display_mode;

    battle_menu_load_selected_unit();
    display_mode = &g_battle_menu_hovered_unit_display_mode;
    *display_mode = 2;
    if (g_battle_menu_active_turn_banner.list_index < 0) {
        *display_mode = 0x0c00;
    }
    battle_menu_display_hovered_unit_stats(&g_battle_menu_hovered_unit_stats_display, UNIT_STATS_PANEL_HOVERED_UNIT,
        (menu_number_entry_t*)((u8*)display_mode - 0x50), &g_battle_menu_active_turn_banner);
}
