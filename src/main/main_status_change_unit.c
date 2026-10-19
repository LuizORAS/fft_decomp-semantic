#include "fft/main.h"

/* Change one byte of the unit's inflicted statuses (MAIN_STATUS_ADD sets status_flag, MAIN_STATUS_REMOVE
 * clears it, MAIN_STATUS_REPLACE stores it) and refresh that byte of the current set (innate |
 * inflicted). No count or graphics change. */
void main_status_change_unit(battle_stats_t* unit, s32 status_byte_index, u8 status_flag, s32 change_type) {
    u8 updated;

    /* An enum selector removes the native signed range test. */
    switch (change_type) {
    case MAIN_STATUS_ADD:
        updated = unit->inflicted_status[status_byte_index] | status_flag;
        break;
    case MAIN_STATUS_REMOVE:
        updated = unit->inflicted_status[status_byte_index] & ~status_flag;
        break;
    case MAIN_STATUS_REPLACE:
        updated = status_flag;
        break;
    }

    unit->inflicted_status[status_byte_index] = updated;
    unit->status_sets.current[status_byte_index] = updated | unit->status_sets.innate[status_byte_index];
}
