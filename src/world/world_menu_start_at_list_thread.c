#include "fft/world.h"
#include "psx/types.h"

/* Runs the g_world_menu_at_list_state menu on the thread two below the current one and
 * waits until that thread and the one below it have finished. */
void world_menu_start_at_list_thread(void) {
    world_menu_entry_t* list = &g_world_menu_at_list_state;
    world_menu_entry_t* saved = g_world_menu_thread_menu_data;

    g_world_menu_at_list_state.select_text_table = 0x13;
    g_world_menu_thread_menu_data = list;
    g_world_menu_at_list_state.text_binding = (struct world_menu_text_binding*)g_world_menu_at_list_entry_table;
    world_menu_init_at_list(list);
    world_thread_start(g_world_thread_current_id - 2, world_build_at_list);
    world_thread_set_parameters(g_world_thread_current_id - 2, (s32)list, 0, 0);
    do {
        world_thread_yield();
    } while (world_thread_is_running_80100164(g_world_thread_current_id - 2) != 0
        || world_thread_is_running_80100164(g_world_thread_current_id - 3) != 0);
    g_world_menu_thread_menu_data = saved;
    world_thread_exit_current();
}
