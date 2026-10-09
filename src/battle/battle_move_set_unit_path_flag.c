#include "fft/battle.h"

void battle_move_set_unit_path_flag(battle_unit_misc_data_t* unit) {
    if (unit->pending_mount == 0) {
        return;
    }
    unit->movement_path[unit->movement_path_offset].value |= 0x10;
}
