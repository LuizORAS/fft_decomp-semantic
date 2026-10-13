#include "fft/menu.h"
#include "fft/sound.h"
#include "fft/world.h"

/* Thread body that opens the battle help screen: reserves 0x20000 bytes at
 * the high overlay address, loads HELPMENU and runs world_menu_run_battle_help.
 * The confirm sound is queued when a unit editor panel is up and no menu
 * window is being built. */
void world_menu_run_helpmenu_thread(void) {
    if (world_thread_find_running_by_task(THREAD_TASK_UNIT_EDITOR_PANEL) != 0
        && world_thread_find_running_by_task(THREAD_TASK_MENU_WINDOW_BUILD) == 0) {
        sound_queue_sfx_confirm();
    }
    menu_reserve_high_overlay(0x20000);
    menu_request_open_companion_executable(COMPANION_OVERLAY_HELPMENU);
    world_menu_run_battle_help();
}
