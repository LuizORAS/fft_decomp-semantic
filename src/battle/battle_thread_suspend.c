#include "fft/battle.h"
#include "psx/types.h"

/* Clear thread_id's running flag; the scheduler skips the slot until battle_thread_resume. */
void battle_thread_suspend(s32 thread_id) {
    g_battle_threads[thread_id].is_running = 0;
}
