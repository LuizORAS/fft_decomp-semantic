#include "fft/battle.h"
#include "fft/script.h"
#include "fft/thread.h"

/* The resident unit-ready predicate always passes, so this instruction never waits. */
void script_wait_for_unit_ready(s32 runtime_id) {
    while (battle_return_one(runtime_id) == 0) {
        thread_yield();
    }
}
