#include "fft/battle.h"

/* Returns 1 when the unit has any current status byte set. */
s32 battle_unit_has_any_current_status(s32 unit_id) {
    battle_stats_t* unit;
    s32 index;

    unit = battle_unit_get_stats_from_battle_id(unit_id);
    for (index = 0; index < BATTLE_STATUS_BYTE_COUNT; index++) {
        if (unit->status_sets.current[index] != 0) {
            return 1;
        }
    }
    return 0;
}
