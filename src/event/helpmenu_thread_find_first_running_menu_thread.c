#include "fft/event_helpmenu.h"
#include "fft/thread.h"

/* The lowest running slot from NATIVE_THREAD_SLOT_SYSTEM_FUNCTION (4) to NATIVE_THREAD_SLOT_MENU (8),
 * or 9 when none runs. */
s32 helpmenu_thread_find_first_running_menu_thread(void) {
    s32 thread_id;
    native_thread_t* thread;

    thread_id = 4;
    thread = g_native_threads + 4;
    do {
        if (thread->is_running == 1) {
            return thread_id;
        }
        thread_id++;
        thread++;
    } while (thread_id < 9);
    return thread_id;
}
