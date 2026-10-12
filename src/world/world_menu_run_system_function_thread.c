#include "fft/world.h"

/* WORLD twin of battle_menu_run_system_function_thread. */
void world_menu_run_system_function_thread(void) {
    s32 id;
    s32 value;
    battle_system_function_t* entry;

    g_world_system_function_thread_busy = 1;
    world_text_init_section_pointers();
    id = g_world_thread_contexts[g_world_thread_current_id].function_parameter_1;
    if (g_main_game_options.fields.navigation_messages != GAME_OPTION_ON) {
        id = g_world_system_function_table[id].alternate_id;
    }
    world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_MENU);
    if (g_world_menu_input_mode == MENU_INPUT_PLAYER) {
        while (world_thread_is_running_asm(NATIVE_THREAD_SLOT_EVENT) != 0) {
            if (g_world_thread_contexts[NATIVE_THREAD_SLOT_EVENT].task_id == NATIVE_THREAD_TASK_EXECUTE_EVENT) {
                world_thread_exit_current();
            }
            world_thread_yield();
        }
    }
    world_camera_wait_until_idle();
    entry = &g_world_system_function_table[id];
    value = entry->menu_id;
    g_world_menu_thread_menu_data[entry->menu_entry_index].text_id = entry->text_id;
    g_world_menu_system_function_row_actions[0] = entry->row_action;
    g_world_menu_current_id = value;
    g_world_menu_thread_menu_data[entry->menu_entry_index].cancel_thread_count = entry->cancel_thread_count;
    if (id == BATTLE_SYSTEM_FUNCTION_CRYSTAL_OR_TREASURE) {
        world_thread_start(NATIVE_THREAD_SLOT_MENU, world_menu_open_option_dead_unit_panel);
        world_thread_set_parameters(NATIVE_THREAD_SLOT_MENU, 0, g_world_menu_dead_unit_panel_parameter, 0);
        world_thread_exit_current();
    }
    if (id != BATTLE_SYSTEM_FUNCTION_CLOSE) {
        world_script_pulse_tutorial_wait_value(id);
    }
    world_thread_start(NATIVE_THREAD_SLOT_MENU, entry->thread_entry);
    world_thread_set_four_parameters(
        NATIVE_THREAD_SLOT_MENU, (s32)&g_world_menu_thread_menu_data[entry->menu_entry_index], 0, 0, 0);
    g_world_system_function_thread_busy = 0;
    world_thread_exit_current();
}
