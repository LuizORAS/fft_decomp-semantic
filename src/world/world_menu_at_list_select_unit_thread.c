#include "fft/thread.h"
#include "fft/world.h"

/* Thread body: pauses the four worker threads after this one, yields until
 * g_world_menu_transition_finished is raised (keeping the g_world_menu_transition_timeout timeout 100 ahead of
 * g_world_menu_at_list_state.selected_index), then resets the state, waits for threads 13..10, points
 * g_text_section_pointers[TEXT_SECTION_AT_LIST] at g_world_menu_at_list_text, resumes the worker threads and
 * stops itself. */
void world_menu_at_list_select_unit_thread(void) {
    g_world_menu_transition_active = 1;
    g_world_menu_overlay_state = 2;
    thread_suspend(g_native_current_thread_id + 1);
    thread_suspend(g_native_current_thread_id + 2);
    thread_suspend(g_native_current_thread_id + 3);
    thread_suspend(g_native_current_thread_id + 4);
    while (g_world_menu_transition_timeout = g_world_menu_at_list_state.selected_index + 100,
        g_world_menu_transition_finished == 0) {
        thread_yield();
    }
    g_world_menu_transition_finished = 0;
    g_world_menu_transition_timeout = 0;
    world_unit_start_view_thread(UNIT_VIEW_MODE_NONE, 0xFF, 0xFF);
    thread_wait_until_inactive(NATIVE_THREAD_SLOT_HOVERED_UNIT_STATS);
    thread_wait_until_inactive(NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS);
    thread_wait_until_inactive(NATIVE_THREAD_SLOT_UNIT_SUMMARY_PANEL);
    thread_wait_until_inactive(NATIVE_THREAD_SLOT_TILE_INFO_PANEL);
    g_text_section_pointers[TEXT_SECTION_AT_LIST] = g_world_menu_at_list_text;
    thread_resume(g_native_current_thread_id + 1);
    thread_resume(g_native_current_thread_id + 2);
    thread_resume(g_native_current_thread_id + 3);
    thread_resume(g_native_current_thread_id + 4);
    g_world_menu_transition_active = 0;
    g_world_menu_overlay_state = 0;
    thread_exit_current();
}
