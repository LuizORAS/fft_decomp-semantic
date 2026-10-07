#include "fft/battle.h"

/* Put the map cursor on the unit with that battle id; returns 0 when there is none. */
s32 battle_target_move_cursor_to_battle_id(u32 battle_id) {
    battle_unit_misc_data_t* unit;

    unit = battle_unit_get_misc_data_by_battle_id(battle_id & 0xFFFF);
    if (unit != 0) {
        battle_target_move_cursor_to_unit(unit);
        return 1;
    }
    return 0;
}
