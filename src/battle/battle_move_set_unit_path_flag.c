#include "fft/battle_move.h"

/* When the unit has a pending mount, mark its current path step as a step onto a unit
 * (BATTLE_MOVE_STEP_ON_UNIT, 0x10). */
void battle_move_set_unit_path_flag(battle_unit_t* unit) {
    if (unit->path.pending_mount == 0) {
        return;
    }
    unit->path.data.steps[unit->movement_path_offset].mount = 1;
}
