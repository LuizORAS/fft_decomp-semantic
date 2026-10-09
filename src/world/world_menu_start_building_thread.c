#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

void world_menu_start_building_thread(void) {
    if (world_thread_is_running_80100164(8) || world_thread_is_running_80100164(NATIVE_THREAD_SLOT_MENU_BUILDING)) {
        if (g_world_menu_current_id != BATTLE_MENU_ID_MINI_MENU) {
            g_world_menu_building_thread_started = 1;
        }
    }
    if (g_world_thread_change_cooldown == 0 && g_world_menu_transition_active == 0 && battle_camera_is_active() == 0
        && g_world_companion_overlay_state == BATTLE_COMPANION_OVERLAY_STATE_IDLE
        && world_thread_is_running_80100164(NATIVE_THREAD_SLOT_MENU_BUILDING) == 0) {
        g_world_text_overlay_active = 1;
        world_thread_start(NATIVE_THREAD_SLOT_MENU_BUILDING, world_menu_run_helpmenu_thread);
        world_thread_set_parameters(NATIVE_THREAD_SLOT_MENU_BUILDING, 0, 0, 0);
    }
}
