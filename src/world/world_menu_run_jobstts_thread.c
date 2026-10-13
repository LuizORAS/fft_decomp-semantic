#include "fft/menu.h"
#include "fft/sound.h"
#include "fft/thread.h"
#include "fft/world.h"

/* Thread body: loads companion overlay file 7 and runs it with state 3 until
 * it reports completion. */
void world_menu_run_jobstts_thread(void) {
    if (world_thread_is_running_asm(NATIVE_THREAD_SLOT_MENU_BUILDING) != 0) {
        thread_exit_current();
    }
    if (world_thread_is_running_asm(NATIVE_THREAD_SLOT_EVENT) != 0) {
        thread_exit_current();
    }
    world_menu_set_transition_active_flag();
    sound_queue_sfx_confirm();
    menu_reserve_high_overlay(0x20000);
    world_menu_request_open_companion_executable(COMPANION_OVERLAY_JOBSTTS);
    thread_wait_frames(2);
    g_world_companion_overlay_state = COMPANION_OVERLAY_STATE_JOBSTTS;
    do {
        g_world_menu_new_button_input = 0;
        thread_yield();
    } while (g_world_companion_overlay_state != COMPANION_OVERLAY_STATE_IDLE);
    g_world_menu_new_button_input = 0;
    menu_free_high_overlay();
    thread_wait_frames(2);
    world_menu_clear_transition_active_flag();
    thread_exit_current();
}
