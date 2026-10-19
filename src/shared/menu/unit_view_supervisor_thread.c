#include "fft/battle_unit.h"
#include "fft/menu.h"
#include "fft/thread.h"
#include "psx/types.h"

enum {
    BATTLE_MENU_UNIT_VIEW_SLOT_IDLE = 0,
    BATTLE_MENU_UNIT_VIEW_SLOT_RESTART = 1,
    BATTLE_MENU_UNIT_VIEW_SLOT_STARTED = 2,
};

/*
 * Unit-view supervisor thread, started by battle_unit_start_view_thread (WORLD's copy by
 * world_unit_start_view_thread).
 *
 * For the requested view mode it starts each missing slot thread, and asks
 * running ones with the wrong task (or a stale viewed unit) to stop, then
 * restarts them once they have exited. Parameter 1 is reset to 0xff so a
 * new request can be detected; the thread exits when none arrives.
 */
void unit_view_supervisor_thread(void) {
    s32 states[16];
    world_menu_unit_view_thread_set_t* set;
    s32 mode;
    s32 i;
    s32 task;
    thread_t* thread;

    for (;;) {
        mode = battle_thread_get_current_parameter_1();
        set = &g_menu_unit_view_thread_sets[mode];
        g_menu_unit_view_mode = mode;
        g_thread_contexts[g_thread_current_id].function_parameter_1 = 0xff;
        for (i = 15; i >= 0; i--) {
            states[i] = BATTLE_MENU_UNIT_VIEW_SLOT_IDLE;
        }
        for (i = 0; i < 4; i++) {
            thread = &g_thread_contexts[set->thread_ids[i]];
            if (thread->is_running != 0) {
                task = thread->task_id;
                if (task == set->task_ids[i]
                    && (task != THREAD_TASK_HOVERED_UNIT_STATS
                        || g_menu_unit_view_cached_battle_id == g_menu_selected_unit.battle_id)
                    && (task != THREAD_TASK_RIGHT_UNIT_STATS
                        || g_menu_unit_view_cached_comparison_id == g_preview_target_unit_id)
                    && (task != THREAD_TASK_UNIT_SUMMARY_PANEL
                        || g_menu_unit_view_cached_battle_id == g_menu_selected_unit.battle_id)) {
                    continue;
                }
                states[set->thread_ids[i]] = BATTLE_MENU_UNIT_VIEW_SLOT_RESTART;
                thread_set_parameters(set->thread_ids[i], 0, 0, 1);
            } else if (set->entries[i] != (void*)-1) {
                thread_start(set->thread_ids[i], set->entries[i]);
                thread_set_parameters(set->thread_ids[i], 0, 0, 0);
                thread->task_id = set->task_ids[i];
                states[set->thread_ids[i]] = BATTLE_MENU_UNIT_VIEW_SLOT_STARTED;
            }
        }
        g_menu_unit_view_cached_battle_id = g_menu_selected_unit.battle_id;
        g_menu_unit_view_cached_comparison_id = g_preview_target_unit_id;
        for (;;) {
            for (i = 0; i < 4; i++) {
                if (states[set->thread_ids[i]] == BATTLE_MENU_UNIT_VIEW_SLOT_RESTART
                    && g_thread_contexts[set->thread_ids[i]].is_running != 0) {
                    break;
                }
            }
            if (i == 4) {
                break;
            }
            thread_wait_frames(1);
        }
        for (i = 0; i < 4; i++) {
            if (states[set->thread_ids[i]] == BATTLE_MENU_UNIT_VIEW_SLOT_RESTART && set->entries[i] != (void*)-1) {
                thread_start(set->thread_ids[i], set->entries[i]);
                thread_set_parameters(set->thread_ids[i], 0, 0, 0);
                g_thread_contexts[set->thread_ids[i]].task_id = set->task_ids[i];
            }
        }
        if (battle_thread_get_current_parameter_1() == 0xff) {
            thread_exit_current();
        }
    }
}
