#include "fft/battle_action.h"
#include "fft/battle_unit.h"

battle_unit_misc_data_t* battle_unit_get_casting_misc_data(void) {
    battle_unit_misc_data_t* unit;

    unit = g_battle_unit_misc_list_head;
    if (unit != 0) {
        do {
            if (unit->unit_id == g_battle_casting_unit_id) {
                return unit;
            }
            unit = unit->previous;
        } while (unit != 0);
    }
    return 0;
}
