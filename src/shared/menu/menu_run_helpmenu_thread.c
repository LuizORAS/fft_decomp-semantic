#include "fft/event_helpmenu.h"
#include "fft/menu.h"
#include "fft/sound.h"
#include "fft/thread.h"

/* Thread that opens the help menu: reserve 0x20000 bytes for the high overlay, load HELPMENU.OUT and
 * run its battle help. The confirm sound is queued when the Status screen is up and no menu window is
 * being built. WORLD compiles this text too. */
void menu_run_helpmenu_thread(void) {
    if (battle_thread_find_running_by_task(THREAD_TASK_STATUS_SCREEN) != 0
        && battle_thread_find_running_by_task(THREAD_TASK_MENU_WINDOW_BUILD) == 0) {
        sound_queue_sfx_confirm();
    }
    menu_reserve_high_overlay(0x20000);
    menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_HELPMENU);
    helpmenu_menu_run_battle_help();
}
