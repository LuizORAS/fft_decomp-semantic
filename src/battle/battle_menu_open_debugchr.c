#include "fft/battle.h"
#include "fft/event_debugchr.h"
#include "psx/types.h"

/* Thread entry of menu entry 0x2c: close the unit panels and the menu thread,
 * load DEBUGCHR.OUT, run its character debug session and restore the menu state
 * afterwards. */
void battle_menu_open_debugchr(void) {
    battle_unit_start_view_thread(UNIT_VIEW_MODE_NONE, 0xFF, 0xFF);
    battle_menu_set_option_menu_open();
    g_battle_menu_suspend_state = 1;
    battle_menu_request_open_companion_executable(COMPANION_OVERLAY_DEBUGCHR);
    battle_thread_set_parameters(NATIVE_THREAD_SLOT_MENU, 0, 0, 1);
    battle_thread_wait_until_inactive(NATIVE_THREAD_SLOT_MENU);
    battle_thread_wait_for_unit_view_panels();
    battle_thread_yield();
    battle_thread_yield();
    debugchr_run_character_debug_session();
    g_battle_menu_suspend_state = 0;
    g_battle_sound_suppressed = 0;
    g_battle_script_event_input = 0;
    battle_menu_clear_option_menu_open();
    battle_thread_exit_current();
}
