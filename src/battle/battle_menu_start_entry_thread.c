#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "fft/battle_menu.h"
#include "fft/battle_script.h"
#include "fft/battle_unit.h"
#include "psx/types.h"

/* Menu thread: waits for thread 8, runs the tutorial event if pending, then
 * starts the menu-entry thread for the requested menu (0x10/0x12 redirect to
 * 0x36 for monster units) and optionally the two follow-up text threads.
 *
 * BATTLE twin of world_menu_start_entry_thread. */
void battle_menu_start_entry_thread(void) {
    s32 menu_id = battle_thread_get_current_parameter_1();
    s32 count;

    battle_thread_wait_until_inactive(NATIVE_THREAD_SLOT_MENU);
    if (g_battle_menu_suspend_state != 0) {
        battle_thread_exit_current();
    }
    camera_wait_until_idle();
    while (battle_thread_find_running_by_task(NATIVE_THREAD_TASK_ATTACK_TARGET_STATS) != 0) {
        battle_thread_yield();
    }
    if (g_battle_menu_input_mode == MENU_INPUT_PLAYER) {
        if (script_is_tutorial_event_slot() != 0) {
            g_battle_menu_input_mode = MENU_INPUT_TUTORIAL;
            script_set_variable(
                EVENT_SCRIPT_VAR_CURRENT_EVENT, script_get_variable(EVENT_SCRIPT_VAR_CURRENT_EVENT) + 1);
            script_start_current_event_thread();
            battle_thread_wait_frames(0x1E);
        }
        if (g_battle_menu_input_mode != MENU_INPUT_PLAYER) {
            battle_thread_wait_frames(2);
        }
    } else {
        battle_thread_wait_frames(2);
    }
    script_pulse_tutorial_wait_value(0xFF);
    if (menu_id == BATTLE_MENU_ID_OTHER_UNIT || menu_id == BATTLE_MENU_ID_OTHER_UNIT_INSPECT) {
        if (battle_unit_get_attacker_data_pointer()->unit_flags & UNIT_FLAG_EGG) {
            menu_id = BATTLE_MENU_ID_EGG;
        }
    }
    battle_thread_start(NATIVE_THREAD_SLOT_MENU, g_battle_menu_thread_menu_data[menu_id].thread_entry);
    battle_thread_set_parameters(NATIVE_THREAD_SLOT_MENU, (s32)&g_battle_menu_thread_menu_data[menu_id], 0, 0);
    count = g_battle_menu_restore_depth;
    g_script_event_input = 0;
    if (g_battle_menu_restore_pending != 0 && g_battle_menu_restore_unit_id == g_battle_active_turn_unit.battle_id) {
        g_battle_menu_help_open = 1;
        battle_thread_wait_frames(2);
        if (count > 0) {
            battle_thread_start(NATIVE_THREAD_SLOT_MENU_CHILD, battle_menu_run_skillset_thread);
            battle_thread_set_parameters(NATIVE_THREAD_SLOT_MENU_CHILD,
                (s32)&g_battle_menu_thread_menu_data[BATTLE_MENU_ID_SKILLSET_LIST], 0, 0);
        }
        battle_thread_wait_frames(2);
        if (count >= 2) {
            battle_thread_start(NATIVE_THREAD_SLOT_MENU_GRANDCHILD, battle_menu_select_unit_action_slots_thread);
            battle_thread_set_parameters(NATIVE_THREAD_SLOT_MENU_GRANDCHILD,
                (s32)&g_battle_menu_thread_menu_data[BATTLE_MENU_ID_ABILITY_LIST], 0, 0);
        }
        battle_thread_wait_frames(2);
        g_battle_menu_restore_pending = 0;
        g_battle_menu_help_open = 0;
    } else {
        g_battle_menu_restore_depth = 0;
        g_battle_menu_restore_unit_id = g_battle_active_turn_unit.battle_id;
    }
    battle_thread_exit_current();
}
