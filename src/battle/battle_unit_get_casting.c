#include "fft/battle_action.h"
#include "fft/battle_unit.h"

/* The runtime record of the casting unit (g_battle_casting_runtime_id), or 0. */
battle_unit_t* battle_unit_get_casting(void) {
    battle_unit_t* unit;

    for (unit = g_battle_unit_list_head; unit != 0; unit = unit->previous) {
        if (unit->runtime_id == g_battle_casting_runtime_id) {
            return unit;
        }
    }
    return 0;
}
