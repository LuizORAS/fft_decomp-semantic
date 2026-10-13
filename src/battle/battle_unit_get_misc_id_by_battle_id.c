#include "fft/battle_unit.h"

/* The misc id of the record whose battle stats occupy unit slot battle_id, or -1. */
s32 battle_unit_get_misc_id_by_battle_id(u32 battle_id) {
    battle_unit_misc_data_t* unit = g_battle_unit_misc_list_head;

    while (unit != 0) {
        battle_stats_t* battle_data = unit->battle_data;

        if (battle_data != 0 && battle_data->battle_id == battle_id) {
            return unit->unit_id;
        }
        unit = unit->previous;
    }
    return -1;
}
