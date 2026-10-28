#include "fft/battle_move.h"
#include "fft/battle_unit.h"
#include "psx/types.h"

/* Event instruction 0x39: is the unit with this runtime id moving (any of the
 * 21 units when the id is -1)? */
s32 unit_is_moving(s32 runtime_id) {
    s32 i;

    if (runtime_id != -1) {
        if (battle_move_is_unit_moving_by_runtime_id(runtime_id) != 0) {
            return 1;
        }
    } else {
        for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
            if ((battle_unit_has_runtime_id(i) != 0) && (battle_move_is_unit_moving_by_runtime_id(i) != 0)) {
                return 1;
            }
        }
    }
    return 0;
}
