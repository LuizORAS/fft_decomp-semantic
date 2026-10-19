#include "fft/thread.h"
#include "fft/world.h"

/* Return the first slot from THREAD_SLOT_SYSTEM_FUNCTION (4) to THREAD_SLOT_MENU
 * (8) whose running flag is exactly 1, or 9 when none is. The battle help menu uses it. */
s32 world_thread_find_running_4_to_8(void) {
    s32 thread_id;

    for (thread_id = THREAD_SLOT_SYSTEM_FUNCTION; thread_id <= THREAD_SLOT_MENU; thread_id++) {
        if (g_threads[thread_id].is_running == 1) {
            return thread_id;
        }
    }
    return thread_id;
}
