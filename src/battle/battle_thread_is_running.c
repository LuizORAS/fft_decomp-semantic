#include "fft/battle.h"
#include "psx/types.h"

/* Return thread_id's running flag. battle_thread_is_running_asm is its hand-assembled twin. */
s32 battle_thread_is_running(s32 thread_id) {
    return g_battle_threads[thread_id].is_running;
}
