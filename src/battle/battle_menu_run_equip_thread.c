#include "fft/battle.h"
#include "psx/pad.h"
#include "psx/types.h"

/*
 * Thread body: loads overlay files 9 and 10, runs the companion overlay
 * with state 4 until it finishes, then restarts menu threads 9..14 and
 * captures the cursors of the icon-list threads 8 and 7.
 *
 * BATTLE twin of world_menu_run_equip_thread.
 */
void battle_menu_run_equip_thread(void) {
    s32 saved;
    s32 i;
    s32 j;

    battle_menu_set_option_menu_open();
    saved = g_battle_unit_view_mode;
    battle_unit_start_view_thread(UNIT_VIEW_MODE_NONE, 0xFF, 0xFF);
    g_battle_menu_suspend_state = 1;
    battle_thread_wait_for_10_to_13();
    battle_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_EQUIP);
    battle_menu_reserve_high_overlay(0x20000);
    battle_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_EQUIP_SECOND_HALF);
    battle_thread_suspend(NATIVE_THREAD_SLOT_MENU);
    battle_thread_suspend(NATIVE_THREAD_SLOT_MENU_CHILD);
    g_battle_companion_overlay_state = BATTLE_COMPANION_OVERLAY_STATE_EQUIP;
    do {
        g_battle_script_event_input = 0;
        battle_thread_yield();
    } while (g_battle_companion_overlay_state != BATTLE_COMPANION_OVERLAY_STATE_IDLE);
    g_battle_menu_thread_menu_data = g_battle_menu_idle_action_entries;
    g_battle_menu_action_slot_row_actions = -3;
    g_battle_sound_suppressed = 1;
    g_battle_script_event_input |= PSX_PAD_CIRCLE;
    if (g_battle_companion_overlay_primitives == 0) {
        battle_thread_resume(NATIVE_THREAD_SLOT_MENU);
        battle_thread_resume(NATIVE_THREAD_SLOT_MENU_CHILD);
    }
    for (i = 9; i < 15; i++) {
        battle_thread_set_parameters(i, 0, 0, 1);
    }
    battle_thread_wait_frames(4);
    if (g_battle_companion_overlay_primitives != 0) {
        for (j = 0; j < 2; j++) {
            g_battle_menu_pending_selection[j]
                = ((world_menu_entry_t*)g_battle_thread_contexts[8 - j].function_parameter_1)->selected_index;
        }
    }
    battle_menu_free_high_overlay();
    g_battle_menu_suspend_state = 0;
    battle_unit_start_view_thread(saved, 0xFF, 0xFF);
    g_battle_sound_suppressed = 0;
    g_battle_script_event_input = 0;
    battle_menu_clear_option_menu_open();
    battle_thread_exit_current();
}
