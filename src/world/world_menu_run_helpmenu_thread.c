#include "fft/world.h"

void world_menu_run_helpmenu_thread(void) {
    if (world_thread_find_running_by_task(NATIVE_THREAD_TASK_UNIT_EDITOR_PANEL) != 0
        && world_thread_find_running_by_task(NATIVE_THREAD_TASK_MENU_WINDOW_BUILD) == 0) {
        world_sound_set_effect_to_confirm();
    }
    world_menu_reserve_high_overlay(0x20000);
    world_menu_request_open_companion_executable(3);
    world_menu_run_battle_help();
}
