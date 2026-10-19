#include "fft/world.h"

/* Ask all eight unit panels of the formation and shop screens to redraw (redraw_request 1): the
 * status and comparison banners, the equipment-and-ability, stats, preview stats, equipment, stat
 * preview and ability panels. */
void world_menu_request_unit_panel_redraw(void) {
    g_world_status_display_thread_params.redraw_request = 1;
    g_world_comparison_display_thread_params.redraw_request = 1;
    g_world_equipment_ability_panel_thread_params.redraw_request = 1;
    g_world_numeric_editor_thread_params.redraw_request = 1;
    g_world_preview_stats_thread_params.redraw_request = 1;
    g_world_change_banner_panel_thread_params.redraw_request = 1;
    g_world_stat_preview_panel_thread_params.redraw_request = 1;
    g_world_ability_panel_thread_params.redraw_request = 1;
}
