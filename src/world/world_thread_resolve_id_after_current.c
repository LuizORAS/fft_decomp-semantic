#include "fft/world.h"

/* Same as world_thread_resolve_id, but the search starts after the current thread's
 * slot. */
s32 world_thread_resolve_id_after_current(s32 requested_thread_id) {
    s32 index;

    if (requested_thread_id < 16) {
        return requested_thread_id;
    }

    index = g_world_thread_current_id + 1;
    if (index >= 17) {
        world_thread_exit_current();
    } else {
        do {
            if (world_thread_is_running_2(index) == 0) {
                return index;
            }
            index++;
        } while (index < 17);

        world_thread_exit_current();
    }
}
