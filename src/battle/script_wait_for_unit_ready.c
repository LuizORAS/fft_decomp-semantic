#include "fft/battle.h"
#include "fft/battle_script.h"
#include "psx/types.h"

/* WaitUnitReady: yield until the unit reports ready. The test is
 * battle_return_one, which always passes, so the instruction never waits. */
void script_wait_for_unit_ready(s32 runtime_id) {
    while (battle_return_one(runtime_id) == 0) {
        battle_thread_yield();
    }
}
