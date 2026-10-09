#include "fft/world.h"
#include "psx/types.h"

void world_formation_stop_menu_threads(void) {
    world_menu_stop_unit_status_banner_thread(NATIVE_THREAD_SLOT_UNIT_STATUS);
    world_menu_stop_unit_status_banner_thread(NATIVE_THREAD_SLOT_COMPARISON_STATUS);
    world_menu_stop_unit_status_banner_thread(WORLD_THREAD_SLOT_EQUIP_ABILITY_PANEL);
    world_menu_stop_unit_status_banner_thread(NATIVE_THREAD_SLOT_NUMERIC_EDITOR);
    world_menu_toggle_preview_stats_window(0);
    world_menu_toggle_stat_preview_panel_thread(0);
    g_world_preview_stats_thread_params.style = 0;
    g_world_stat_preview_panel_thread_params.style = 0;
    g_world_equipment_ability_panel_thread_params.style = 0;
    g_world_ability_panel_thread_params.style = 0;
}
