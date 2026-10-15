#include "fft/main.h"

/* Set up a unit's statuses for battle: inflicted = current minus innate, the counts and death
 * counter cleared (main_status_init_ct), then the flags reconciled
 * (main_status_update_unit_flags_and_ct). */
void main_status_init_unit(battle_stats_t* unit) {
    s32 status_set;

    for (status_set = 0; status_set < BATTLE_STATUS_BYTE_COUNT; status_set++) {
        unit->inflicted_status[status_set]
            = unit->status_sets.current[status_set] & ~unit->status_sets.innate[status_set];
    }
    main_status_init_ct(unit);
    main_status_update_unit_flags_and_ct(unit);
}
