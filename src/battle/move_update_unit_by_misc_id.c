#include "fft/battle_move.h"
#include "psx/types.h"

/* Advance the movement of every misc unit (ids 0-15) that has a path by one
 * frame. */
void move_update_unit_by_misc_id(void) {
    s32 i;
    battle_unit_t* unit;

    for (i = 0; i < BATTLE_UNIT_RUNTIME_SLOT_COUNT; i++) {
        unit = battle_unit_get_misc_data_by_misc_id(i);
        if (unit != 0) {
            if (unit->path.count != 0) {
                move_update_path_step(unit);
            }
        }
    }
}
