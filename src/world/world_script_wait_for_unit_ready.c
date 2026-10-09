#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

void world_script_wait_for_unit_ready(s32 misc_id) {
    while (battle_return_one(misc_id) == 0) {
        world_thread_yield();
    }
}
