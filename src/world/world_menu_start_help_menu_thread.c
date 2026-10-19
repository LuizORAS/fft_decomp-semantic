#include "fft/battle_camera.h"
#include "fft/thread.h"
#include "fft/world.h"

/* WORLD twin of battle_menu_start_help_menu_thread. */
void world_menu_start_help_menu_thread(void) {
    if (world_thread_is_running_asm(THREAD_SLOT_MENU) || world_thread_is_running_asm(THREAD_SLOT_MENU_BUILDING)) {
        if (g_menu_current_id != BATTLE_MENU_ID_MINI_MENU) {
            g_world_menu_building_thread_started = 1;
        }
    }
    if (g_world_event_input_suppression_frames == 0 && g_world_menu_help_blocked == 0 && battle_camera_is_active() == 0
        && g_world_companion_overlay_state == BATTLE_COMPANION_OVERLAY_STATE_IDLE
        && world_thread_is_running_asm(THREAD_SLOT_MENU_BUILDING) == 0) {
        g_world_menu_help_open = 1;
        thread_start(THREAD_SLOT_MENU_BUILDING, world_menu_run_helpmenu_thread);
        thread_set_parameters(THREAD_SLOT_MENU_BUILDING, 0, 0, 0);
    }
}
