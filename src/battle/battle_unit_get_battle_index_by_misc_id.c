#include "fft/battle_unit.h"

/* The battle unit slot of a misc record, or -1 when the record has no battle
 * stats. A misc id without a record raises pointer exception 12 and also returns
 * -1. */
s32 battle_unit_get_battle_index_by_misc_id(u32 misc_id) {
    battle_unit_misc_data_t* unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);

    if (unit != 0) {
        battle_stats_t* battle_data = unit->battle_data;

        if (battle_data != 0) {
            return battle_data->battle_id;
        }
    } else {
        main_system_handle_pointer_exception(12);
    }
    return -1;
}
