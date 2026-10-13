#include "fft/thread.h"

/* Resolve a thread slot argument: a real slot id is returned unchanged; any
 * other value picks the first idle slot after the current thread, up to slot
 * 16. With none idle the current thread exits. */
s32 world_thread_resolve_id_after_current(s32 thread_id) {
    s32 index;

    if (thread_id < NATIVE_THREAD_SLOT_COUNT) {
        return thread_id;
    }

    index = g_native_current_thread_id + 1;
    if (index >= 17) {
        thread_exit_current();
    } else {
        do {
            if (world_thread_is_running_asm(index) == 0) {
                return index;
            }
            index++;
        } while (index < 17);

        thread_exit_current();
    }
}
