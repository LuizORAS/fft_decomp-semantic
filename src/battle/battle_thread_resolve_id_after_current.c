#include "fft/thread.h"
#include "psx/types.h"

/* Same as battle_thread_resolve_id, but the search starts after the current thread's
 * slot. */
s32 battle_thread_resolve_id_after_current(s32 requested_thread_id) {
    s32 i;

    if (requested_thread_id < THREAD_SLOT_COUNT) {
        return requested_thread_id;
    }
    i = g_thread_current_id + 1;
    if (i < THREAD_SLOT_COUNT) {
        do {
            if (battle_thread_is_running_asm(i) == 0) {
                return i;
            }
            i++;
        } while (i < THREAD_SLOT_COUNT);
    }
    thread_exit_current();
}
