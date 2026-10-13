#include "fft/event_attack.h"
#include "psx/types.h"

/* Return thread_id's running flag in BATTLE's thread table; twin of battle_thread_is_running. */
s32 attack_thread_is_running(s32 thread_id) {
    return g_battle_threads[thread_id].is_running;
}
