#include "fft/battle.h"
#include "psx/types.h"

void battle_menu_start_building_thread(void) {
    if (battle_thread_is_running_8014cc94(8) != 0
        || battle_thread_is_running_8014cc94(NATIVE_THREAD_SLOT_MENU_BUILDING) != 0) {
        if (g_battle_menu_current_id != BATTLE_MENU_ID_MINI_MENU) {
            g_battle_menu_building_thread_started = 1;
        }
    }
    if (g_battle_event_input_suppression_frames != 0) {
        return;
    }
    if (g_option_menu_open != 0) {
        return;
    }
    if (battle_camera_is_active() != 0) {
        return;
    }
    if (g_battle_companion_overlay_state != BATTLE_COMPANION_OVERLAY_STATE_IDLE) {
        return;
    }
    if (battle_thread_is_running_8014cc94(NATIVE_THREAD_SLOT_MENU_BUILDING) != 0) {
        return;
    }
    g_battle_menu_help_open = 1;
    battle_thread_start(NATIVE_THREAD_SLOT_MENU_BUILDING, battle_menu_run_helpmenu_thread);
    battle_thread_set_parameters(NATIVE_THREAD_SLOT_MENU_BUILDING, 0, 0, 0);
}
