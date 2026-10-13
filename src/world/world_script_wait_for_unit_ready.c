#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

/* Yield until battle_return_one reports the unit ready; that function always
 * returns 1, so this never waits. */
void world_script_wait_for_unit_ready(s32 runtime_id) {
    while (battle_return_one(runtime_id) == 0) {
        world_thread_yield();
    }
}
