#include "fft/battle.h"

/* Called for the last step of a path: when the unit has a pending mount, set the
 * step's mount bit (0x10), which marks a step that ends on another unit. */
void battle_move_set_unit_path_flag(battle_unit_misc_data_t* unit) {
    if (unit->pending_mount == 0) {
        return;
    }
    unit->movement_path[unit->movement_path_offset].value |= 0x10;
}
