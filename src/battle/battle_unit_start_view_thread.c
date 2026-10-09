#include "fft/battle.h"
#include "psx/types.h"

void battle_unit_start_view_thread(s32 event_id, s32 unit_a, s32 unit_b) {
    if (g_battle_menu_suspend_state == 1) {
        return;
    }
    battle_text_build_unit_name_list();
    g_battle_menu_panel_fade_mode = g_battle_menu_panel_fade_modes[event_id];
    if (unit_a != 0xff) {
        g_battle_active_turn_unit.battle_id = unit_a;
        g_battle_menu_active_turn_banner.unit_index = unit_a;
    }
    if (unit_b != 0xff) {
        g_battle_preview_target_unit_id = unit_b;
        g_battle_menu_status_billboard.unit_index = unit_b;
    }
    if (battle_thread_is_running_8014cc94(2) == 0) {
        battle_thread_start(2, (s32)battle_unit_view_supervisor_thread);
    }
    battle_thread_set_parameters(2, event_id, 0, 0);
}
