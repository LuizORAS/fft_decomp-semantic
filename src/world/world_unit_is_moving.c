#include "fft/battle_move.h"
#include "fft/battle_unit.h"
#include "fft/world.h"
#include "psx/types.h"

/* Return whether one unit, or any existing unit when id is -1, is moving. */
s32 world_unit_is_moving(s32 id) {
    s32 i;

    if (id != -1) {
        if (move_is_unit_moving_by_runtime_id(id) != 0) {
            return 1;
        }
    } else {
        for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i += 1) {
            if (battle_unit_has_runtime_id(i) != 0 && move_is_unit_moving_by_runtime_id(i) != 0) {
                return 1;
            }
        }
    }
    return 0;
}
