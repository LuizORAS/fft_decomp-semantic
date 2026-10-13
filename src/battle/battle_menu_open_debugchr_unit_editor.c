#include "fft/battle_menu.h"
#include "fft/battle_script.h"
#include "fft/event_debugchr.h"
#include "fft/menu.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Show the active-turn unit in DEBUGCHR.OUT's unit editor when the unit has mount
 * info (it is a rider or a mount); for any other unit it returns at once and the
 * caller's own panel is used.
 *
 * The editor replaces the unit panels and the menu thread; afterwards the idle
 * action menu is rebuilt, the unit summary view restarts and the calling thread
 * exits. */
void battle_menu_open_debugchr_unit_editor(void) {
    s16* selected = &g_battle_active_turn_unit.battle_id;

    if (battle_unit_get_stats_from_battle_id(*selected)->mount_info == 0) {
        return;
    }
    battle_unit_start_view_thread(UNIT_VIEW_MODE_NONE, 0xFF, 0xFF);
    battle_menu_set_modal_open();
    g_battle_menu_suspend_state = 1;
    menu_request_open_companion_executable(COMPANION_OVERLAY_DEBUGCHR);
    thread_set_parameters(THREAD_SLOT_MENU, 0, 0, 1);
    thread_wait_until_inactive(THREAD_SLOT_MENU);
    thread_wait_for_unit_view_panels();
    battle_menu_record_selection_for_selected_unit();
    battle_menu_enter_status_screen_selection();
    debugchr_editor_run_unit_thread(*selected);
    g_battle_menu_suspend_state = 0;
    g_sound_suppressed = 0;
    g_script_event_input = 0;
    battle_menu_build_idle_action_menu(g_battle_menu_current_id);
    battle_unit_start_view_thread(UNIT_VIEW_MODE_UNIT_SUMMARY, *selected, *selected);
    battle_menu_clear_modal_open();
    battle_menu_leave_status_screen_selection();
    thread_exit_current();
}
