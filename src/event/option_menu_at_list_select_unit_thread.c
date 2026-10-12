#include "fft/event_option.h"
#include "psx/types.h"

/* Close the AT list's worker set and restore the parent option threads. */
void option_menu_at_list_select_unit_thread(void) {
    g_battle_menu_help_blocked = 1;
    g_battle_menu_suspend_state = 2;

    battle_thread_suspend(g_battle_thread_current_id + 1);
    battle_thread_suspend(g_battle_thread_current_id + 2);
    battle_thread_suspend(g_battle_thread_current_id + 3);
    battle_thread_suspend(g_battle_thread_current_id + 4);

    while ((g_option_menu_transition_timer = g_option_menu_transition_duration + 100),
        g_battle_menu_at_list_unit_view_ended == 0) {
        battle_thread_yield();
    }

    g_battle_menu_at_list_unit_view_ended = 0;
    g_option_menu_transition_timer = 0;
    battle_unit_start_view_thread(UNIT_VIEW_MODE_NONE, 0xff, 0xff);

    battle_thread_wait_until_inactive(NATIVE_THREAD_SLOT_HOVERED_UNIT_STATS);
    battle_thread_wait_until_inactive(NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS);
    battle_thread_wait_until_inactive(NATIVE_THREAD_SLOT_UNIT_SUMMARY_PANEL);
    battle_thread_wait_until_inactive(NATIVE_THREAD_SLOT_TILE_INFO_PANEL);

    g_battle_text_section_pointers[TEXT_SECTION_AT_LIST] = g_option_menu_at_list_text;
    battle_thread_resume(g_battle_thread_current_id + 1);
    battle_thread_resume(g_battle_thread_current_id + 2);
    battle_thread_resume(g_battle_thread_current_id + 3);
    battle_thread_resume(g_battle_thread_current_id + 4);

    g_battle_menu_help_blocked = 0;
    g_battle_menu_suspend_state = 0;
    battle_thread_exit_current();
}
