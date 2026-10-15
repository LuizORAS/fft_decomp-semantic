#include "fft/world.h"

/* WORLD twin of battle_menu_run_helpmenu_thread. */
void world_menu_run_helpmenu_thread(void) {
    if (world_thread_find_running_by_task(NATIVE_THREAD_TASK_STATUS_SCREEN) != 0
        && world_thread_find_running_by_task(NATIVE_THREAD_TASK_MENU_WINDOW_BUILD) == 0) {
        world_sound_set_effect_to_confirm();
    }
    world_menu_reserve_high_overlay(0x20000);
    world_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_HELPMENU);
    world_menu_run_battle_help();
}
