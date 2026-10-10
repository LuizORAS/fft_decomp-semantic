#include "fft/battle.h"
#include "psx/types.h"

/* As battle_thread_resolve_id, but the search for an idle slot starts above the
 * current thread's slot. */
s32 battle_thread_resolve_id_after_current(s32 requested_thread_id) {
    s32 i;

    if (requested_thread_id < NATIVE_THREAD_SLOT_COUNT) {
        return requested_thread_id;
    }
    i = g_battle_current_thread_id + 1;
    if (i < NATIVE_THREAD_SLOT_COUNT) {
        do {
            if (battle_thread_is_running_asm(i) == 0) {
                return i;
            }
            i++;
        } while (i < NATIVE_THREAD_SLOT_COUNT);
    }
    battle_thread_exit_current();
}
