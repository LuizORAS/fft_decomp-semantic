#include "fft/battle.h"
#include "psx/types.h"

s32 battle_thread_resolve_id(s32 requested_thread_id) {
    s32 i;

    if (requested_thread_id < NATIVE_THREAD_SLOT_COUNT) {
        return requested_thread_id;
    }
    i = 1;
    do {
        if (battle_thread_is_running_asm(i) == 0) {
            return i;
        }
        i++;
    } while (i < NATIVE_THREAD_SLOT_COUNT);
    battle_thread_exit_current();
}
