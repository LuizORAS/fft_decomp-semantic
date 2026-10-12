#include "fft/battle.h"
#include "psx/types.h"

/* Run one frame of the event script and the menus and return the frame's menu selection
 * (battle_menu_resolve_selection's packed command, option and item). Under tutorial or blocked input
 * the unfiltered controller input drives the frame. While a menu thread runs (slots 6 to 8, the mini
 * menu excepted) the event speed is forced to 2, a VSync interval of 2 or 30 frames a second, and the
 * speed in use comes back when they stop.
 *
 * The `fill` staging variable places the -2 load ahead of the loop counter the way the target does; a
 * literal store hoists it after the counter. */
s32 battle_menu_update_menus_and_event_speed(u32* otag, s32 controller_input) {
    s32 i;
    s32 result;
    s32 fill;

    g_battle_ai_workspace_ptr = (world_event_work_t*)g_battle_ai_workspace;
    if (g_battle_menu_input_mode != MENU_INPUT_PLAYER) {
        controller_input = g_battle_script_unfiltered_controller_input;
    }
    battle_map_store_selected_tile_data();
    fill = -2;
    for (i = 3; i >= 0; i--) {
        g_battle_menu_pending_selection[i] = fill;
    }
    battle_script_run_event_frame(otag, controller_input);
    battle_menu_update_panel_fade();
    result = battle_menu_resolve_selection();
    if ((battle_thread_is_running_asm(NATIVE_THREAD_SLOT_MENU) != 0
            && g_battle_thread_contexts[NATIVE_THREAD_SLOT_MENU].task_id != NATIVE_THREAD_TASK_MINI_MENU)
        || battle_thread_is_running_asm(NATIVE_THREAD_SLOT_MENU_CHILD) != 0
        || battle_thread_is_running_asm(NATIVE_THREAD_SLOT_MENU_GRANDCHILD) != 0) {
        if (g_battle_animation_speed_forced == 0) {
            g_battle_animation_speed_forced = 1;
            g_battle_saved_animation_speed = battle_state_get_vsync_interval();
            battle_script_set_event_speed(2);
        }
    } else if (g_battle_animation_speed_forced == 1) {
        g_battle_animation_speed_forced = 0;
        battle_script_set_event_speed(g_battle_saved_animation_speed);
    }
    return result;
}
