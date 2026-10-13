#include "fft/battle_move.h"
#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

/* Event instruction 0x29 WaitWalk: yield until the unit stops moving. */
void world_script_waitwalk_event_instruction(s32 unit_id) {
    s32 misc_id;

    misc_id = script_get_misc_id(unit_id);
    if (misc_id != EVENT_RUNTIME_ID_NONE) {
        do {
            world_thread_yield();
        } while (move_is_unit_moving_by_misc_id(misc_id) != 0);
    }
}
