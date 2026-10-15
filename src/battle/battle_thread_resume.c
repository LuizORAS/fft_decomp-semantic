#include "fft/battle.h"
#include "psx/types.h"

/* Set thread_id's running flag again, so the scheduler continues it from its last yield. */
void battle_thread_resume(s32 thread_id) {
    g_battle_threads[thread_id].is_running = 1;
}
