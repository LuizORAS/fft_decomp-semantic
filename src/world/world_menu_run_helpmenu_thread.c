#include "fft/world.h"

/* Thread body that opens the battle help screen: reserves 0x20000 bytes at
 * the high overlay address, loads HELPMENU and runs world_menu_run_battle_help.
 * The confirm sound is queued when a unit editor panel is up and no menu
 * window is being built. */
void world_menu_run_helpmenu_thread(void) {
    if (world_thread_find_running_by_task(NATIVE_THREAD_TASK_STATUS_SCREEN) != 0
        && world_thread_find_running_by_task(NATIVE_THREAD_TASK_MENU_WINDOW_BUILD) == 0) {
        world_sound_set_effect_to_confirm();
    }
    world_menu_reserve_high_overlay(0x20000);
    world_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_HELPMENU);
    world_menu_run_battle_help();
}
