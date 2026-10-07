#include "fft/battle.h"
#include "psx/types.h"

/* Advance the path step of every unit (misc ids 0-15) that has a path; the event states call
 * it each frame. */
void battle_move_update_all_walking_units(void) {
    s32 i;
    battle_unit_misc_data_t* unit;

    i = 0;
    do {
        unit = battle_unit_get_misc_data_by_misc_id((u16)i);
        if (unit != 0) {
            if (unit->movement_path_count != 0) {
                battle_move_update_path_step(unit);
            }
        }
        i++;
    } while (i < 0x10);
}
