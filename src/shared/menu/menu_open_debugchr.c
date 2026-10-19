#include "fft/battle_menu.h"
#include "fft/battle_unit.h"
#include "fft/event_debugchr.h"
#include "fft/menu.h"
#include "fft/script.h"
#include "fft/sound.h"
#include "fft/thread.h"

/* Thread entry of menu entry 0x2c: close the unit panels and the menu thread,
 * load DEBUGCHR.OUT, run its character debug session and restore the menu state
 * afterwards. WORLD compiles this text too. */
void menu_open_debugchr(void) {
    battle_unit_start_view_thread(UNIT_VIEW_MODE_NONE, 0xFF, 0xFF);
    battle_menu_set_modal_open();
    g_menu_overlay_state = 1;
    menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_DEBUGCHR);
    thread_set_parameters(THREAD_SLOT_MENU, 0, 0, 1);
    thread_wait_until_inactive(THREAD_SLOT_MENU);
    thread_wait_for_unit_view_panels();
    thread_yield();
    thread_yield();
    debugchr_editor_run_session();
    g_menu_overlay_state = 0;
    g_sound_suppressed = 0;
    g_script_event_input = 0;
    battle_menu_clear_modal_open();
    thread_exit_current();
}
