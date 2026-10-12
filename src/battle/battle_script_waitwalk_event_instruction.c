#include "fft/battle_event.h"
#include "fft/battle_move.h"
#include "fft/battle_thread.h"

/* WaitWalk: yield, at least once, until the unit has no movement path left. A
 * unit id that does not resolve returns at once. */
void battle_script_waitwalk_event_instruction(s32 unit_id) {
    s32 misc_id = battle_script_get_misc_id(unit_id);
    if (misc_id != EVENT_MISC_ID_NONE) {
        do {
            battle_thread_yield();
        } while (move_is_unit_moving_by_misc_id(misc_id) != 0);
    }
}
