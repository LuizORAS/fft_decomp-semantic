#include "fft/wldcore.h"
#include "psx/types.h"

void wldcore_start_core_background_threads(s32 party_index) {
    wldcore_thread_block_t* thread8;

    wldcore_unit_build_status_panel_data(party_index);
    thread8 = &g_wldcore_thread8_params;
    g_wldcore_thread9_params.y = 0;
    g_wldcore_threadc_params.y = 0;
    g_wldcore_thread8_params.y = 0;
    thread8->x = 0;
    world_thread_start(8, world_menu_unit_status_banner_thread);
    world_thread_set_parameters(8, (s32)thread8, 0, 0);
    world_thread_start(0xC, world_menu_run_numeric_editor_thread);
    world_thread_set_parameters(0xC, (s32)&g_wldcore_threadc_params, 0, 0);
    world_thread_start(9, world_menu_equipment_and_ability_panel_thread);
    world_thread_set_parameters(9, (s32)&g_wldcore_thread9_params, 0, 0);
    g_main_system_flags |= 0x20000;
}
