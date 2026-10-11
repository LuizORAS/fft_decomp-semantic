#include "fft/battle.h"
#include "psx/types.h"

/*
 * Battle twin of world_menu_run_system_function_thread. Thread started by
 * battle_menu_start_system_function_thread: waits for the menu
 * threads to settle, then configures the menu entry named by the selected
 * system-function record and starts that record's thread as thread 8.
 */
void battle_menu_run_system_function_thread(void) {
    s32 id;
    s32 value;
    battle_system_function_t* entry;

    g_battle_system_function_thread_busy = 1;
    battle_text_init_menu_section_pointers();
    id = g_battle_thread_contexts[g_battle_thread_current_id].function_parameter_1;
    if (g_main_game_options.fields.navigation_messages != GAME_OPTION_ON) {
        id = g_battle_system_function_table[id].alternate_id;
    }
    battle_thread_wait_until_inactive(NATIVE_THREAD_SLOT_MENU);
    if (g_battle_menu_input_mode == MENU_INPUT_PLAYER) {
        while (battle_thread_is_running_asm(NATIVE_THREAD_SLOT_EVENT) != 0) {
            if (g_battle_thread_contexts[NATIVE_THREAD_SLOT_EVENT].task_id == NATIVE_THREAD_TASK_EXECUTE_EVENT) {
                battle_thread_exit_current();
            }
            battle_thread_yield();
        }
    }
    battle_camera_wait_until_idle();
    entry = &g_battle_system_function_table[id];
    value = entry->menu_id;
    g_battle_menu_thread_menu_data[entry->menu_entry_index].text_id = entry->text_id;
    g_battle_menu_system_function_row_actions[0] = entry->row_action;
    g_battle_menu_current_id = value;
    g_battle_menu_thread_menu_data[entry->menu_entry_index].cancel_thread_count = entry->cancel_thread_count;
    if (id == 0x1f) {
        battle_thread_start(NATIVE_THREAD_SLOT_MENU, battle_menu_open_option_dead_unit_panel);
        battle_thread_set_parameters(NATIVE_THREAD_SLOT_MENU, 0, g_battle_menu_dead_unit_panel_parameter, 0);
        battle_thread_exit_current();
    }
    if (id != 8) {
        battle_script_pulse_tutorial_wait_value(id);
    }
    battle_thread_start(NATIVE_THREAD_SLOT_MENU, entry->thread_entry);
    battle_thread_set_four_parameters(
        NATIVE_THREAD_SLOT_MENU, (s32)&g_battle_menu_thread_menu_data[entry->menu_entry_index], 0, 0, 0);
    g_battle_system_function_thread_busy = 0;
    battle_thread_exit_current();
}
