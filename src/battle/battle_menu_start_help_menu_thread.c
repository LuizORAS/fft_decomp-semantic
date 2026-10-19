#include "fft/battle_camera.h"
#include "fft/battle_event.h"
#include "fft/battle_menu.h"
#include "fft/battle_unit.h"
#include "fft/thread.h"

/* Start the help menu (battle_menu_run_helpmenu_thread) in the menu-building slot.
 *
 * If the menu or menu-building thread is running and the current menu is not the mini menu,
 * g_battle_menu_building_thread_started is set first, which makes the next
 * battle_menu_build_idle_action_menu call do nothing. The help starts only when event input is not
 * suppressed, the help is not blocked (battle_menu_set_modal_open), the camera is idle, no companion overlay
 * is running and the building slot is free. */
void battle_menu_start_help_menu_thread(void) {
    if (battle_thread_is_running_asm(THREAD_SLOT_MENU) != 0
        || battle_thread_is_running_asm(THREAD_SLOT_MENU_BUILDING) != 0) {
        if (g_menu_current_id != BATTLE_MENU_ID_MINI_MENU) {
            g_battle_menu_building_thread_started = 1;
        }
    }
    if (g_battle_event_input_suppression_frames != 0) {
        return;
    }
    if (g_battle_menu_modal_open != 0) {
        return;
    }
    if (battle_camera_is_active() != 0) {
        return;
    }
    if (g_battle_companion_overlay_state != BATTLE_COMPANION_OVERLAY_STATE_IDLE) {
        return;
    }
    if (battle_thread_is_running_asm(THREAD_SLOT_MENU_BUILDING) != 0) {
        return;
    }
    g_battle_menu_help_open = 1;
    thread_start(THREAD_SLOT_MENU_BUILDING, battle_menu_run_helpmenu_thread);
    thread_set_parameters(THREAD_SLOT_MENU_BUILDING, 0, 0, 0);
}
