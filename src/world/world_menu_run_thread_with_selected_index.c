#include "fft/world.h"

/* world_menu_run_thread, first setting the entry's selected index when no menu thread is running.
 * Never called. */
s32 world_menu_run_thread_with_selected_index(s32 thread_id, world_menu_entry_t* arg, s16 value) {
    if (g_world_menu_thread_running_status == 0) {
        arg->selected_index = value;
    }
    return world_menu_run_thread(thread_id, arg);
}
