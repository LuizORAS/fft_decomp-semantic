#include "fft/battle_unit.h"

/* The misc (display) record whose unit_id is misc_id, or 0. */
battle_unit_misc_data_t* battle_unit_get_misc_data_by_misc_id(u16 misc_id) {
    battle_unit_misc_data_t* unit;

    unit = g_battle_unit_misc_list_head;
    while (unit != 0) {
        if (unit->unit_id == misc_id) {
            return unit;
        }
        unit = unit->previous;
    }
    return 0;
}
