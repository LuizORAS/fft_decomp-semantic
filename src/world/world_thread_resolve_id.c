#include "fft/thread.h"

/* Return requested_thread_id when it is a slot number (below 16); otherwise the first stopped
 * slot among 1-16. With every slot running, the calling thread exits instead (QUIRKS.md). */
s32 world_thread_resolve_id(s32 requested_thread_id) {
    s32 index;

    if (requested_thread_id < THREAD_SLOT_COUNT) {
        return requested_thread_id;
    }

    for (index = 1; index < 17; index++) {
        if (world_thread_is_running_asm(index) == 0) {
            return index;
        }
    }

    thread_exit_current();
}
