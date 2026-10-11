#include "fft/event_debugchr.h"
#include "fft/world.h"

/* Target 0x800f001c. Loads the DEBUGCHR overlay (file 0xB) and runs the unit
 * debug editor thread for the stored unit when it has mount info set. */
void world_menu_open_debugchr_unit_editor(void) {
    s16* unit_index = &g_world_active_unit_data.battle_id;

    if (battle_unit_get_stats_from_battle_id(*unit_index)->mount_info != 0) {
        world_unit_start_view_thread(UNIT_VIEW_MODE_NONE, 0xFF, 0xFF);
        world_menu_set_transition_active_flag();
        g_world_menu_overlay_state = 1;
        world_menu_request_open_companion_executable(COMPANION_OVERLAY_DEBUGCHR);
        world_thread_set_parameters(NATIVE_THREAD_SLOT_MENU, 0, 0, 1);
        world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_MENU);
        world_thread_wait_for_unit_view_panels();
        world_menu_record_selection_for_selected_unit();
        battle_menu_enter_status_screen_selection();
        debugchr_editor_run_unit_thread(*unit_index);
        g_world_menu_overlay_state = 0;
        g_world_menu_sound_muted = 0;
        g_world_menu_new_button_input = 0;
        world_menu_build_idle_action_stub(g_world_menu_current_id);
        world_unit_start_view_thread(UNIT_VIEW_MODE_UNIT_SUMMARY, *unit_index, *unit_index);
        world_menu_clear_transition_active_flag();
        battle_menu_leave_status_screen_selection();
        world_thread_exit_current();
    }
}
