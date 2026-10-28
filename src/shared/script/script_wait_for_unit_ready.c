#include "fft/battle.h"
#include "fft/script.h"
#include "fft/thread.h"

/* WaitUnitReady: yield until the unit reports ready. The test is
 * battle_return_one, which always passes, so the instruction never waits. BATTLE and WORLD compile this
 * text. */
void script_wait_for_unit_ready(s32 runtime_id) {
    while (battle_return_one(runtime_id) == 0) {
        thread_yield();
    }
}
