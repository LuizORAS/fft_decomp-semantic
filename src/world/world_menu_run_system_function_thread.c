#include "fft/thread.h"
#include "fft/world.h"

/*
 * Thread started by world_menu_start_system_function_thread: waits for the menu
 * threads to settle, then configures the menu entry named by the selected
 * system-function record and starts that record's thread as thread 8.
 */
void world_menu_run_system_function_thread(void) {
    s32 id;
    s32 menu_id;
    menu_system_function_t* entry;

    g_world_system_function_thread_busy = 1;
    world_text_init_section_pointers();
    id = g_native_thread_contexts[g_native_current_thread_id].function_parameter_1;
    if (g_main_game_options.fields.navigation_messages != GAME_OPTION_ON) {
        id = g_world_system_function_table[id].alternate_id;
    }
    thread_wait_until_inactive(NATIVE_THREAD_SLOT_MENU);
    if (g_world_menu_input_mode == MENU_INPUT_PLAYER) {
        while (world_thread_is_running_asm(NATIVE_THREAD_SLOT_EVENT) != 0) {
            if (g_native_thread_contexts[NATIVE_THREAD_SLOT_EVENT].task_id == NATIVE_THREAD_TASK_EXECUTE_EVENT) {
                thread_exit_current();
            }
            thread_yield();
        }
    }
    world_camera_wait_until_idle();
    entry = &g_world_system_function_table[id];
    menu_id = entry->menu_id;
    g_world_menu_thread_menu_data[entry->menu_entry_index].text_id = entry->text_id;
    g_world_menu_system_function_row_actions[0] = entry->row_action;
    g_world_menu_current_id = menu_id;
    g_world_menu_thread_menu_data[entry->menu_entry_index].thread_count = entry->thread_count;
    if (id == 0x1f) {
        thread_start(NATIVE_THREAD_SLOT_MENU, world_menu_open_option_dead_unit_panel);
        thread_set_parameters(NATIVE_THREAD_SLOT_MENU, 0, g_world_menu_dead_unit_panel_parameter, 0);
        thread_exit_current();
    }
    if (id != 8) {
        world_script_pulse_tutorial_wait_value(id);
    }
    thread_start(NATIVE_THREAD_SLOT_MENU, entry->thread_entry);
    thread_set_four_parameters(
        NATIVE_THREAD_SLOT_MENU, (s32)&g_world_menu_thread_menu_data[entry->menu_entry_index], 0, 0, 0);
    g_world_system_function_thread_busy = 0;
    thread_exit_current();
}
