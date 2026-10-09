#include "fft/world.h"
#include "psx/types.h"

/* Thread body: loads companion overlay file 7 and runs it with state 3 until
 * it reports completion. */
void world_menu_run_jobstts_thread(void) {
    if (world_thread_is_running_80100164(NATIVE_THREAD_SLOT_MENU_BUILDING) != 0) {
        world_thread_exit_current();
    }
    if (world_thread_is_running_80100164(1) != 0) {
        world_thread_exit_current();
    }
    world_menu_set_transition_active_flag();
    world_sound_set_effect_to_confirm();
    world_menu_reserve_high_overlay(0x20000);
    world_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_JOBSTTS);
    world_thread_wait_frames(2);
    g_world_companion_overlay_state = BATTLE_COMPANION_OVERLAY_STATE_JOBSTTS;
    do {
        g_world_menu_new_button_input = 0;
        world_thread_yield();
    } while (g_world_companion_overlay_state != BATTLE_COMPANION_OVERLAY_STATE_IDLE);
    g_world_menu_new_button_input = 0;
    world_menu_free_high_overlay();
    world_thread_wait_frames(2);
    world_menu_clear_transition_active_flag();
    world_thread_exit_current();
}
