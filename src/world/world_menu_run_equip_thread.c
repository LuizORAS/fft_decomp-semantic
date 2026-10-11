#include "fft/world.h"
#include "psx/pad.h"
#include "psx/types.h"

/*
 * Thread body: loads overlay files 9 and 10, runs the companion overlay
 * with state 4 until it finishes, then restarts menu threads 9..14 and
 * captures the cursors of the icon-list threads 8 and 7.
 */
void world_menu_run_equip_thread(void) {
    s32 saved;
    s32 i;
    s32 j;

    world_menu_set_transition_active_flag();
    saved = g_world_unit_view_mode;
    world_unit_start_view_thread(UNIT_VIEW_MODE_NONE, 0xFF, 0xFF);
    g_world_menu_overlay_state = 1;
    world_thread_wait_for_unit_view_panels();
    world_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_EQUIP);
    world_menu_reserve_high_overlay(0x20000);
    world_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_EQUIP_SECOND_HALF);
    world_thread_suspend(NATIVE_THREAD_SLOT_MENU);
    world_thread_suspend(NATIVE_THREAD_SLOT_MENU_CHILD);
    g_world_companion_overlay_state = BATTLE_COMPANION_OVERLAY_STATE_EQUIP;
    do {
        g_world_menu_new_button_input = 0;
        world_thread_yield();
    } while (g_world_companion_overlay_state != BATTLE_COMPANION_OVERLAY_STATE_IDLE);
    g_world_menu_thread_menu_data = g_world_menu_system_entries;
    g_world_menu_action_slot_row_actions[0] = -3;
    g_world_menu_sound_muted = 1;
    g_world_menu_new_button_input |= PSX_PAD_CIRCLE;
    if (g_world_companion_overlay_prims == 0) {
        world_thread_resume(NATIVE_THREAD_SLOT_MENU);
        world_thread_resume(NATIVE_THREAD_SLOT_MENU_CHILD);
    }
    for (i = 9; i < 15; i++) {
        world_thread_set_parameters(i, 0, 0, 1);
    }
    world_thread_wait_frames(4);
    if (g_world_companion_overlay_prims != 0) {
        for (j = 0; j < 2; j++) {
            g_world_menu_pending_selection[j]
                = ((world_menu_entry_t*)g_world_thread_contexts[8 - j].function_parameter_1)->selected_index;
        }
    }
    world_menu_free_high_overlay();
    g_world_menu_overlay_state = 0;
    world_unit_start_view_thread(saved, 0xFF, 0xFF);
    g_world_menu_sound_muted = 0;
    g_world_menu_new_button_input = 0;
    world_menu_clear_transition_active_flag();
    world_thread_exit_current();
}
