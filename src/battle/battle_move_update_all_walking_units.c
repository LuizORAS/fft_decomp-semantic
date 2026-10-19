#include "fft/battle_move.h"
#include "psx/types.h"

/* Advance the path step of every unit (runtime ids 0-15) that has a path; the event states call
 * it each frame. */
void battle_move_update_all_walking_units(void) {
    s32 i;
    battle_unit_t* unit;

    for (i = 0; i < BATTLE_UNIT_RUNTIME_SLOT_COUNT; i++) {
        unit = battle_unit_get_by_runtime_id(i);
        if (unit != 0 && unit->path.count != 0) {
            battle_move_update_path_step(unit);
        }
    }
}
