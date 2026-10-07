#include "fft/battle.h"

/* Mark a mounted unit's current path step as a step onto a unit (BATTLE_MOVE_STEP_ON_UNIT). */
void battle_move_set_unit_path_flag(battle_unit_misc_data_t* unit) {
    if (unit->mount_byte == 0) {
        return;
    }
    unit->movement_path[unit->movement_path_offset] |= BATTLE_MOVE_STEP_ON_UNIT;
}
