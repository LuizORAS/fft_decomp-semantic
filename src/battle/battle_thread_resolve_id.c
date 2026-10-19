#include "fft/thread.h"
#include "psx/types.h"

/* Return requested_thread_id when it is a slot number (below 16); otherwise the first stopped
 * slot from 1. With every slot running, the calling thread exits instead (QUIRKS.md). */
s32 battle_thread_resolve_id(s32 requested_thread_id) {
    s32 i;

    if (requested_thread_id < THREAD_SLOT_COUNT) {
        return requested_thread_id;
    }
    i = 1;
    do {
        if (battle_thread_is_running_asm(i) == 0) {
            return i;
        }
        i++;
    } while (i < THREAD_SLOT_COUNT);
    thread_exit_current();
}
