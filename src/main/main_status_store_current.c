#include "fft/main.h"

/* Rebuild the unit's current statuses, byte by byte: innate | inflicted. */
void main_status_store_current(battle_stats_t* unit) {
    s32 status_set;

    for (status_set = 0; status_set < BATTLE_STATUS_BYTE_COUNT; status_set++) {
        unit->status_sets.current[status_set]
            = unit->status_sets.innate[status_set] | unit->inflicted_status[status_set];
    }
}
