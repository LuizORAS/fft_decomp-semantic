#include "fft/battle_target.h"

/* Move the cursor to the unit with this battle id. Returns 1 when the unit has
 * misc data, otherwise 0 with the cursor unchanged. */
s32 target_move_cursor_to_battle_id(u32 battle_id) {
    battle_unit_t* unit;

    unit = battle_unit_get_misc_data_by_battle_id(battle_id & 0xFFFF);
    if (unit != 0) {
        target_move_cursor_to_unit(unit);
        return 1;
    }
    return 0;
}
