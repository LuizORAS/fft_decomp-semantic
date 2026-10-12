#include "fft/battle_ai.h"
#include "fft/battle_status.h"

/*
 * Reject removal of a control status that expires before the unit's next turn.
 *
 * Only Charm, Don't Move, and Don't Act are filtered; other IDs return true.
 * Equality counts as lasting through the turn. This does not test presence.
 */
s32 ai_check_unit_gets_turn_without_status(battle_stats_t* unit, s32 status) {
    if (status == STATUS_ID_CHARM || (status - STATUS_ID_DONT_MOVE) < 2U) {
        if (ai_calculate_clockticks_until_unit_acts(unit) > unit->status_ct[STATUS_CT_INDEX(status)]) {
            return 0;
        }
    }
    return 1;
}
