#include "fft/world.h"

/* The lowest running slot from NATIVE_THREAD_SLOT_SYSTEM_FUNCTION (4) to NATIVE_THREAD_SLOT_MENU (8),
 * or 9 when none runs. */
s32 world_thread_find_first_running_menu_thread(void) {
    s32 thread_id;

    for (thread_id = NATIVE_THREAD_SLOT_SYSTEM_FUNCTION; thread_id <= NATIVE_THREAD_SLOT_MENU; thread_id++) {
        if (g_world_threads[thread_id].is_running == 1) {
            return thread_id;
        }
    }
    return thread_id;
}
