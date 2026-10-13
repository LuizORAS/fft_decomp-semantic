#include "fft/battle_menu.h"
#include "fft/battle_script.h"
#include "fft/battle_sound.h"
#include "fft/battle_unit.h"
#include "fft/menu.h"
#include "fft/sound.h"
#include "fft/thread.h"

/* Thread body that swaps in companion overlay 7 behind the option menu. */
void battle_menu_run_jobstts_thread(void) {
    if (battle_thread_is_running_asm(NATIVE_THREAD_SLOT_MENU_BUILDING) != 0) {
        thread_exit_current();
    }
    if (battle_thread_is_running_asm(NATIVE_THREAD_SLOT_EVENT) != 0) {
        thread_exit_current();
    }
    battle_menu_set_modal_open();
    sound_queue_sfx_confirm();
    menu_reserve_high_overlay(0x20000);
    menu_request_open_companion_executable(COMPANION_OVERLAY_JOBSTTS);
    thread_wait_frames(2);
    g_battle_companion_overlay_state = COMPANION_OVERLAY_STATE_JOBSTTS;
    do {
        g_script_event_input = 0;
        thread_yield();
    } while (g_battle_companion_overlay_state != COMPANION_OVERLAY_STATE_IDLE);
    g_script_event_input = 0;
    menu_free_high_overlay();
    thread_wait_frames(2);
    battle_menu_clear_modal_open();
    thread_exit_current();
}
