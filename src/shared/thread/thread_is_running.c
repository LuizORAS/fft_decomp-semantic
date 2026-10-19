#include "fft/thread.h"

/* Return thread_id's running flag. battle_thread_is_running_asm is its hand-assembled twin. */
s32 thread_is_running(s32 thread_id) {
    return g_threads[thread_id].is_running;
}
