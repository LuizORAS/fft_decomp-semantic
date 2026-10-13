#include "fft/battle_menu.h"
#include "fft/event_helpmenu.h"
#include "fft/menu.h"
#include "fft/sound.h"

/* Thread that opens the help menu: reserve 0x20000 bytes for the high overlay,
 * load HELPMENU.OUT and run its battle help. The confirm sound plays first when
 * the unit editor panel is up and no menu window is being built. */
void battle_menu_run_helpmenu_thread(void) {
    if (battle_thread_find_running_by_task(THREAD_TASK_UNIT_EDITOR_PANEL) != 0
        && battle_thread_find_running_by_task(THREAD_TASK_MENU_WINDOW_BUILD) == 0) {
        sound_queue_sfx_confirm();
    }
    menu_reserve_high_overlay(0x20000);
    menu_request_open_companion_executable(COMPANION_OVERLAY_HELPMENU);
    helpmenu_menu_run_battle_help();
}
