#include "fft/battle.h"

/* When the unit has a pending mount, mark its current path step as a step onto a unit
 * (BATTLE_MOVE_STEP_ON_UNIT, 0x10). */
void battle_move_set_unit_path_flag(battle_unit_misc_data_t* unit) {
    if (unit->pending_mount == 0) {
        return;
    }
    unit->movement_path[unit->movement_path_offset].value |= 0x10;
}
