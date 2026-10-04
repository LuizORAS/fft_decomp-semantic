#include "fft/world.h"

/* Return requested_thread_id when it is a slot number (below 16); otherwise the first stopped
 * slot among 1-16. With every slot running, the calling thread exits instead (QUIRKS.md). */
s32 world_thread_resolve_id(s32 requested_thread_id) {
    s32 index;

    if (requested_thread_id < 16) {
        return requested_thread_id;
    }

    index = 1;
    do {
        if (world_thread_is_running_2(index) == 0) {
            return index;
        }
        index++;
    } while (index < 17);

    world_thread_exit_current();
}
