#include "fft/battle_menu.h"
#include "fft/event_debugchr.h"
#include "fft/menu.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "fft/unit.h"
#include "fft/world.h"

/* Show the active unit's Status screen with DEBUGCHR.OUT (debugchr_editor_run_unit_thread, which
 * uses DEBUGCHR's panels read-only) when the unit has mount info, a rider or a mount; for any other unit
 * it returns at once and battle_menu_run_status_screen_thread draws its own. WORLD compiles this text
 * too.
 *
 * The screen replaces the unit panels and the menu thread; afterwards the idle action menu is rebuilt,
 * the unit summary view restarts and the calling thread exits. */
void menu_open_rider_status_screen(void) {
    s16* unit_index = &g_menu_selected_unit.battle_id;

    if (battle_unit_get_stats_from_battle_id(*unit_index)->mount_info != 0) {
        world_unit_start_view_thread(UNIT_VIEW_MODE_NONE, 0xFF, 0xFF);
        world_menu_block_help();
        g_menu_overlay_state = 1;
        menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_DEBUGCHR);
        thread_set_parameters(THREAD_SLOT_MENU, 0, 0, 1);
        thread_wait_until_inactive(THREAD_SLOT_MENU);
        thread_wait_for_unit_view_panels();
        world_menu_record_selection_for_selected_unit();
        battle_menu_enter_status_screen_selection();
        debugchr_editor_run_unit_thread(*unit_index);
        g_menu_overlay_state = 0;
        g_world_sound_suppressed = 0;
        g_script_event_input = 0;
        world_menu_build_idle_action_stub(g_menu_current_id);
        world_unit_start_view_thread(UNIT_VIEW_MODE_UNIT_SUMMARY, *unit_index, *unit_index);
        world_menu_unblock_help();
        battle_menu_leave_status_screen_selection();
        thread_exit_current();
    }
}
