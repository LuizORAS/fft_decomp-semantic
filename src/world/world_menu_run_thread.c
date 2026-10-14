#include "fft/thread.h"
#include "fft/world.h"

/* Start-or-poll helper for a menu thread: the first call starts
 * `thread_id` on the entry's thread function with the entry as parameter 1
 * (unless the slot is still busy) and returns 1; later calls return whether
 * the thread is still running, 0 once it has ended. */
s32 world_menu_run_thread(s32 thread_id, menu_entry_t* arg) {
    s32 status;
    void (*fn)(void);

    if (g_world_menu_thread_running_status == 0) {
        if (thread_is_running(thread_id) != 0) {
            return 1;
        }
        fn = arg->thread_entry;
        g_menu_thread_menu_data = arg;
        thread_start(thread_id, fn);
        thread_set_parameters(thread_id, (s32)g_menu_thread_menu_data, 0, 0);
        g_world_menu_thread_running_status = 1;
        return 1;
    }
    status = thread_is_running(thread_id);
    g_world_menu_thread_running_status = status;
    return status;
}
