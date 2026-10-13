#include "fft/menu.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Menu thread: waits for thread 8, runs the tutorial event if pending, then
 * starts the menu-entry thread for the requested menu (0x10/0x12 redirect to
 * 0x36 for monster units) and optionally the two follow-up text threads. */
void world_menu_start_entry_thread(void) {
    s32 menu_id = (s32)world_thread_get_current_parameter_1();
    s32 count;

    world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_MENU);
    if (g_world_menu_overlay_state != 0) {
        world_thread_exit_current();
    }
    world_camera_wait_until_idle();
    while (world_thread_find_running_by_task(NATIVE_THREAD_TASK_ATTACK_TARGET_STATS) != 0) {
        thread_yield();
    }
    if (g_world_menu_input_mode == MENU_INPUT_PLAYER) {
        if (world_script_check_tutorial_event_slot() != 0) {
            g_world_menu_input_mode = MENU_INPUT_TUTORIAL;
            world_script_set_variable(
                EVENT_SCRIPT_VAR_CURRENT_EVENT, script_get_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT) + 1);
            world_script_start_current_event_thread();
            world_thread_wait_frames(0x1E);
        }
        if (g_world_menu_input_mode != MENU_INPUT_PLAYER) {
            world_thread_wait_frames(2);
        }
    } else {
        world_thread_wait_frames(2);
    }
    world_script_pulse_tutorial_wait_value(0xFF);
    if (menu_id == BATTLE_MENU_ID_OTHER_UNIT || menu_id == BATTLE_MENU_ID_OTHER_UNIT_INSPECT) {
        if (world_unit_get_battle_stats_for_stored()->unit_flags & UNIT_FLAG_EGG) {
            menu_id = BATTLE_MENU_ID_EGG;
        }
    }
    world_thread_start(NATIVE_THREAD_SLOT_MENU, g_world_menu_thread_menu_data[menu_id].thread_entry);
    world_thread_set_parameters(NATIVE_THREAD_SLOT_MENU, (s32)&g_world_menu_thread_menu_data[menu_id], 0, 0);
    count = g_world_menu_restore_depth;
    g_world_menu_new_button_input = 0;
    if (g_menu_restore_pending != 0 && g_world_menu_restore_unit_id == g_world_active_unit_data.battle_id) {
        g_world_text_overlay_active = 1;
        world_thread_wait_frames(2);
        if (count > 0) {
            world_thread_start(NATIVE_THREAD_SLOT_MENU_CHILD, world_menu_run_skillset_thread);
            world_thread_set_parameters(
                NATIVE_THREAD_SLOT_MENU_CHILD, (s32)&g_world_menu_thread_menu_data[BATTLE_MENU_ID_SKILLSET_LIST], 0, 0);
        }
        world_thread_wait_frames(2);
        if (count >= 2) {
            world_thread_start(NATIVE_THREAD_SLOT_MENU_GRANDCHILD, world_menu_select_unit_action_slots_thread);
            world_thread_set_parameters(NATIVE_THREAD_SLOT_MENU_GRANDCHILD,
                (s32)&g_world_menu_thread_menu_data[BATTLE_MENU_ID_ABILITY_LIST], 0, 0);
        }
        world_thread_wait_frames(2);
        g_menu_restore_pending = 0;
        g_world_text_overlay_active = 0;
    } else {
        g_world_menu_restore_depth = 0;
        g_world_menu_restore_unit_id = g_world_active_unit_data.battle_id;
    }
    world_thread_exit_current();
}
