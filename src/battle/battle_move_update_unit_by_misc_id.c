#include "fft/battle.h"
#include "psx/types.h"

/* Advance the movement of every misc unit (ids 0-15) that has a path by one
 * frame. */
void battle_move_update_unit_by_misc_id(void) {
    s32 i;
    battle_unit_misc_data_t* unit;

    i = 0;
    do {
        unit = battle_unit_get_misc_data_by_misc_id(i);
        if (unit != 0) {
            if (unit->path.count != 0) {
                battle_move_update_path_step(unit);
            }
        }
        i++;
    } while (i < 0x10);
}
