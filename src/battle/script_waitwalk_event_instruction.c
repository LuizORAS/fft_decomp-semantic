#include "fft/battle_move.h"
#include "fft/battle_script.h"
#include "fft/battle_thread.h"
#include "fft/script.h"
#include "fft/thread.h"

/* WaitWalk: yield, at least once, until the unit has no movement path left. A
 * unit id that does not resolve returns at once. */
void script_waitwalk_event_instruction(s32 unit_id) {
    s32 runtime_id = script_resolve_runtime_id(unit_id);
    if (runtime_id != EVENT_RUNTIME_ID_NONE) {
        do {
            thread_yield();
        } while (move_is_unit_moving_by_runtime_id(runtime_id) != 0);
    }
}
