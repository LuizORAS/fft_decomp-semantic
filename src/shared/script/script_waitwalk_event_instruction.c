#include "fft/battle_move.h"
#include "fft/script.h"
#include "fft/thread.h"

/* Yield at least once, then wait until the resolved unit stops moving. */
void script_waitwalk_event_instruction(s32 unit_id) {
    s32 runtime_id;

    runtime_id = script_resolve_runtime_id(unit_id);
    if (runtime_id != EVENT_RUNTIME_ID_NONE) {
        do {
            thread_yield();
        } while (move_is_unit_moving_by_runtime_id(runtime_id) != 0);
    }
}
