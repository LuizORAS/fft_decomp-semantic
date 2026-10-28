#include "fft/battle_action.h"
#include "fft/battle_unit.h"

/* The runtime record of the unit the battle states act for (g_battle_unit_source_runtime_id), or 0. */
battle_unit_t* battle_unit_get_source(void) {
    battle_unit_t* unit;
    s32 source_id;

    unit = g_battle_unit_list_head;
    if (unit != 0) {
        source_id = g_battle_unit_source_runtime_id;
        for (; unit != 0; unit = unit->previous) {
            if (unit->runtime_id == source_id) {
                return unit;
            }
        }
    }
    return 0;
}
