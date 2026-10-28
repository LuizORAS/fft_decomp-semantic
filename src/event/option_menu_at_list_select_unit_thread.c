#include "fft/event_option.h"
#include "fft/thread.h"

/* Thread of the AT list's second menu record, which each unit row opens (row action 1): suspend the
 * four threads in the slots above it and hold the windows' transition timer at its duration + 100 while the
 * player views the picked unit, until BATTLE raises g_battle_menu_at_list_unit_view_ended
 * (battle_menu_end_at_list_unit_view, from battle_state_return_to_mini_menu). Then close the unit view,
 * wait for the four unit panels to stop, point the AT list text section back at the list's names, resume
 * the four threads and exit. */
void option_menu_at_list_select_unit_thread(void) {
    g_battle_menu_modal_open = 1;
    g_menu_overlay_state = 2;

    thread_suspend(g_thread_current_id + 1);
    thread_suspend(g_thread_current_id + 2);
    thread_suspend(g_thread_current_id + 3);
    thread_suspend(g_thread_current_id + 4);

    while ((g_option_menu_transition_timer = g_option_menu_transition_duration + 100),
        g_battle_menu_at_list_unit_view_ended == 0) {
        thread_yield();
    }

    g_battle_menu_at_list_unit_view_ended = 0;
    g_option_menu_transition_timer = 0;
    battle_unit_start_view_thread(UNIT_VIEW_MODE_NONE, 0xff, 0xff);

    thread_wait_until_inactive(THREAD_SLOT_HOVERED_UNIT_STATS);
    thread_wait_until_inactive(THREAD_SLOT_RIGHT_UNIT_STATS);
    thread_wait_until_inactive(THREAD_SLOT_UNIT_SUMMARY_PANEL);
    thread_wait_until_inactive(THREAD_SLOT_TILE_INFO_PANEL);

    g_text_section_pointers[TEXT_SECTION_AT_LIST] = g_option_menu_at_list_text;
    thread_resume(g_thread_current_id + 1);
    thread_resume(g_thread_current_id + 2);
    thread_resume(g_thread_current_id + 3);
    thread_resume(g_thread_current_id + 4);

    g_battle_menu_modal_open = 0;
    g_menu_overlay_state = 0;
    thread_exit_current();
}
