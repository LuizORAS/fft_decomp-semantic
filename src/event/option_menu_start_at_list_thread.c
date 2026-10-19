#include "fft/event_option.h"
#include "fft/thread.h"

/* Run the AT list: make g_option_menu_at_list_state the menu record (menu type 0x13), build its rows
 * from the turn order (option_menu_init_at_list), start option_menu_run_list_thread two slots below, and
 * yield until it and the thread below it (the unit view a row opens) have finished; then restore the
 * previous menu record and exit. */
void option_menu_start_at_list_thread(void) {
    world_menu_entry_t* previous_menu_descriptor = g_menu_thread_menu_data;

    g_option_menu_at_list_menu_type = 0x13;
    g_menu_thread_menu_data = g_option_menu_at_list_state;
    g_option_menu_at_list_entry_table_pointer = g_option_menu_at_list_entry_table;
    option_menu_init_at_list(g_option_menu_at_list_state);

    thread_start(g_thread_current_id - 2, option_menu_run_list_thread);
    thread_set_parameters(g_thread_current_id - 2, g_option_menu_at_list_state, 0, 0);

    do {
        thread_yield();
    } while (battle_thread_is_running_asm(g_thread_current_id - 2) != 0
        || battle_thread_is_running_asm(g_thread_current_id - 3) != 0);

    g_menu_thread_menu_data = previous_menu_descriptor;
    thread_exit_current();
}
