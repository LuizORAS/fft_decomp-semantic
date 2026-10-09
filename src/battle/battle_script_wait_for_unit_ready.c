#include "fft/battle.h"
#include "psx/types.h"

void battle_script_wait_for_unit_ready(s32 misc_id) {
    while (battle_return_one(misc_id) == 0) {
        battle_thread_yield();
    }
}
