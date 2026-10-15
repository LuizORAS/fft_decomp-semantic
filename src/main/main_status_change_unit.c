#include "fft/main.h"

/* Change one byte of the unit's inflicted statuses (MAIN_STATUS_ADD sets status_flag, MAIN_STATUS_REMOVE
 * clears it, MAIN_STATUS_REPLACE stores it) and refresh that byte of the current set (innate |
 * inflicted). No count or graphics change. */
void main_status_change_unit(battle_stats_t* unit, s32 status_set, u8 status_flag, s32 change_type) {
    u8 flag = status_flag;
    u8 updated;
    u8 current;

    switch (change_type) {
    case MAIN_STATUS_ADD:
        updated = unit->inflicted_status[status_set] | status_flag;
        break;
    case MAIN_STATUS_REMOVE:
        updated = unit->inflicted_status[status_set] & ~status_flag;
        break;
    case MAIN_STATUS_REPLACE:
        updated = flag;
        break;
    }

    current = unit->status_sets.innate[status_set];
    unit->inflicted_status[status_set] = updated;
    unit->status_sets.current[status_set] = updated | current;
}
