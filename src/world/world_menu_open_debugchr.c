#include "fft/event_debugchr.h"
#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_open_debugchr. */
void world_menu_open_debugchr(void) {
    world_unit_start_view_thread(UNIT_VIEW_MODE_NONE, 0xFF, 0xFF);
    world_menu_block_help();
    g_world_menu_suspend_state = 1;
    world_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_DEBUGCHR);
    world_thread_set_parameters(NATIVE_THREAD_SLOT_MENU, 0, 0, 1);
    world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_MENU);
    world_thread_wait_for_unit_view_panels();
    world_thread_yield();
    world_thread_yield();
    debugchr_run_character_debug_session();
    g_world_menu_suspend_state = 0;
    g_world_sound_suppressed = 0;
    g_world_menu_new_button_input = 0;
    world_menu_unblock_help();
    world_thread_exit_current();
}
