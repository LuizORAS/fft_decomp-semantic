#include "fft/battle_menu.h"
#include "fft/battle_script.h"
#include "fft/battle_sound.h"
#include "fft/battle_thread.h"
#include "fft/battle_unit.h"

/* Thread body that swaps in companion overlay 7 behind the option menu. */
void battle_menu_run_jobstts_thread(void) {
    if (battle_thread_is_running_asm(NATIVE_THREAD_SLOT_MENU_BUILDING) != 0) {
        battle_thread_exit_current();
    }
    if (battle_thread_is_running_asm(NATIVE_THREAD_SLOT_EVENT) != 0) {
        battle_thread_exit_current();
    }
    battle_menu_set_option_menu_open();
    sound_set_effect_to_confirm();
    battle_menu_reserve_high_overlay(0x20000);
    battle_menu_request_open_companion_executable(COMPANION_OVERLAY_JOBSTTS);
    battle_thread_wait_frames(2);
    g_battle_companion_overlay_state = COMPANION_OVERLAY_STATE_JOBSTTS;
    do {
        g_script_event_input = 0;
        battle_thread_yield();
    } while (g_battle_companion_overlay_state != COMPANION_OVERLAY_STATE_IDLE);
    g_script_event_input = 0;
    battle_menu_free_high_overlay();
    battle_thread_wait_frames(2);
    battle_menu_clear_option_menu_open();
    battle_thread_exit_current();
}
