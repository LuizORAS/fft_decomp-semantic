#include "fft/event_debugchr.h"
#include "fft/world.h"
#include "psx/types.h"

void world_menu_open_debugchr(void) {
    world_unit_start_view_thread(UNIT_VIEW_MODE_NONE, 0xFF, 0xFF);
    world_menu_set_transition_active_flag();
    g_world_menu_overlay_state = 1;
    world_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_DEBUGCHR);
    world_thread_set_parameters(NATIVE_THREAD_SLOT_MENU, 0, 0, 1);
    world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_MENU);
    world_thread_wait_for_10_to_13();
    world_thread_yield();
    world_thread_yield();
    debugchr_run_character_debug_session();
    g_world_menu_overlay_state = 0;
    g_world_menu_sound_muted = 0;
    g_world_menu_new_button_input = 0;
    world_menu_clear_transition_active_flag();
    world_thread_exit_current();
}
