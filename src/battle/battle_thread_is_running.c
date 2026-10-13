#include "fft/battle_thread.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Returns thread_id's running flag: the compiled counterpart of the hand-written
 * battle_thread_is_running_asm. */
s32 battle_thread_is_running(s32 thread_id) {
    return g_native_threads[thread_id].is_running;
}
