#include "fft/battle_status.h"
#include "fft/main.h"
#include "psx/types.h"

/* Apply Float and reconcile the unit's status flags and status CT. */
void main_status_update_unit_flags_and_ct(battle_stats_t* unit) {
    s32 status;
    u8 immunity;

    if (unit->movement_abilities[2] & BATTLE_MOVEMENT_SET_3_FLOAT) {
        unit->status_sets.innate[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FLOAT)]
            |= BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FLOAT);
    }

    for (status = 0; status < BATTLE_STATUS_BYTE_COUNT; status++) {
        immunity = unit->status_sets.immunity[status];
        immunity &= ~unit->status_sets.innate[status];
        unit->status_sets.immunity[status] = immunity;
        unit->status_sets.current[status] = unit->status_sets.innate[status] | unit->inflicted_status[status];
    }

    for (status = BATTLE_STATUS_ID_POISON; status < BATTLE_STATUS_COUNT; status++) {
        s32 status_byte = status / 8;
        s32 status_bit = status & 7;

        if ((unit->inflicted_status[status_byte] & (0x80 >> status_bit))
            && unit->status_ct[BATTLE_STATUS_CT_INDEX(status)] == 0) {
            main_status_set_ct(unit, status, 0);
        }
    }
}
